#include "acl_stdafx.hpp"
#ifndef ACL_PREPARE_COMPILE
#include "acl_cpp/stdlib/log.hpp"
#include "acl_cpp/stdlib/util.hpp"
#include "acl_cpp/stream/socket_stream.hpp"
#include "acl_cpp/master/master_udp.hpp"
#endif

namespace acl
{

static master_udp* __mu = NULL;

master_udp::master_udp(void)
{
	// ȫ�־�̬����
	acl_assert(__mu == NULL);
	__mu = this;
}

master_udp::~master_udp(void)
{
	close_sstreams();
}

void master_udp::close_sstreams(void)
{
	std::vector<socket_stream*>::iterator it = sstreams_.begin();
	for (; it != sstreams_.end(); ++it)
	{
		// ���� daemon ����ģʽ��socket_stream �е� ACL_VSTREAM
		// ������ lib_acl ���е� acl_udp_server �ڲ��رգ�����
		// �˴���Ҫ�� ACL_VSTREAM ������ socket_stream ����
		if (daemon_mode_)
			(*it)->unbind();
		delete *it;
	}

	// ������������ϣ���Ϊ�ú���������ʱ����������һ��
	sstreams_.clear();
}

static bool has_called = false;

void master_udp::run_daemon(int argc, char** argv)
{
#ifdef ACL_WINDOWS
	logger_fatal("no support win32 yet!");
#else
	// ÿ������ֻ����һ��ʵ��������
	acl_assert(has_called == false);
	has_called = true;
	daemon_mode_ = true;

	// ���� acl ����������� UDP ������ģ��ӿ�
	acl_udp_server_main(argc, argv, service_main,
		ACL_MASTER_SERVER_PRE_INIT, service_pre_jail,
		ACL_MASTER_SERVER_POST_INIT, service_init,
		ACL_MASTER_SERVER_EXIT, service_exit,
		ACL_MASTER_SERVER_THREAD_INIT, thread_init,
		ACL_MASTER_SERVER_THREAD_EXIT, thread_exit,
		ACL_MASTER_SERVER_INT_TABLE, conf_.get_int_cfg(),
		ACL_MASTER_SERVER_STR_TABLE, conf_.get_str_cfg(),
		ACL_MASTER_SERVER_BOOL_TABLE, conf_.get_bool_cfg(),
		ACL_MASTER_SERVER_INT64_TABLE, conf_.get_int64_cfg(),
		0);
#endif
}

//////////////////////////////////////////////////////////////////////////

bool master_udp::run_alone(const char* addrs, const char* path /* = NULL */,
	unsigned int)
{
#ifdef ACL_WINDOWS
	acl_cpp_init();
#endif

	// ÿ������ֻ����һ��ʵ��������
	acl_assert(has_called == false);
	has_called = true;
	daemon_mode_ = false;
	acl_assert(addrs && *addrs);

	int  argc = 0;
	const char *argv[6];

	const char* proc = acl_process_path();
	argv[argc++] = proc ? proc : "demo";
	argv[argc++] = "-n";
	argv[argc++] = addrs;
	if (path && *path)
	{
		argv[argc++] = "-f";
		argv[argc++] = path;
	}
	argv[argc++] = "-r";

	acl_udp_server_main(argc, (char**) argv, service_main,
		ACL_MASTER_SERVER_PRE_INIT, service_pre_jail,
		ACL_MASTER_SERVER_POST_INIT, service_init,
		ACL_MASTER_SERVER_EXIT, service_exit,
		ACL_MASTER_SERVER_THREAD_INIT, thread_init,
		ACL_MASTER_SERVER_THREAD_EXIT, thread_exit,
		ACL_MASTER_SERVER_INT_TABLE, conf_.get_int_cfg(),
		ACL_MASTER_SERVER_STR_TABLE, conf_.get_str_cfg(),
		ACL_MASTER_SERVER_BOOL_TABLE, conf_.get_bool_cfg(),
		ACL_MASTER_SERVER_INT64_TABLE, conf_.get_int64_cfg(),
		0);

	return true;
}

//////////////////////////////////////////////////////////////////////////

static void on_close(ACL_VSTREAM* stream, void* ctx)
{
	if (ctx && stream->context == ctx)
	{
		socket_stream* ss = (socket_stream*) ctx;
		ss->unbind();
		delete ss;
	}
}

void master_udp::service_main(ACL_VSTREAM *stream, char*, char**)
{
	acl_assert(__mu != NULL);

	socket_stream* ss = (socket_stream*) stream->context;
	if (ss == NULL)
	{
		// ����������һ�α�����ʱ����Ҫ�� socket_stream ��
		ss = NEW socket_stream();
		if (ss->open(stream) == false)
			logger_fatal("open stream error!");
		stream->context = ss;
		acl_vstream_add_close_handle(stream, on_close, ss);
	}

/*
#ifndef	ACL_WINDOWS
	if (__mu->daemon_mode_)
		acl_watchdog_pat();  // ����֪ͨ acl_master ���һ��
#endif
*/
	__mu->on_read(ss);
}

void master_udp::service_pre_jail(char*, char**)
{
	acl_assert(__mu != NULL);

#ifndef ACL_WINDOWS
	ACL_EVENT* eventp = acl_udp_server_event();
	__mu->set_event(eventp);
#endif

	__mu->proc_pre_jail();
}

void master_udp::service_init(char*, char**)
{
	acl_assert(__mu != NULL);

	__mu->proc_inited_ = true;
	__mu->proc_on_init();
}

void master_udp::service_exit(char*, char**)
{
	acl_assert(__mu != NULL);
	__mu->proc_on_exit();
}

void master_udp::thread_init(void *)
{
#ifndef	ACL_WINDOWS
	if (__mu->daemon_mode_)
	{
		ACL_VSTREAM** streams = acl_udp_server_streams();
		__mu->locker_.lock();
		if (streams != NULL)
		{
			for (int i = 0; streams[i] != NULL; i++)
			{
				socket_stream* ss = NEW socket_stream();
				if (ss->open(streams[i]) == false)
					logger_fatal("open stream error!");
				__mu->sstreams_.push_back(ss);
			}
		}
		__mu->locker_.unlock();
	}
#endif
	__mu->thread_on_init();
}

void master_udp::thread_exit(void *)
{
	__mu->thread_on_exit();
}

}  // namespace acl
