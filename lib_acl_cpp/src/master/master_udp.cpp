#include "acl_stdafx.hpp"
#ifndef ACL_PREPARE_COMPILE
#include "acl_cpp/stdlib/log.hpp"
#include "acl_cpp/stdlib/util.hpp"
#include "acl_cpp/stream/socket_stream.hpp"
#include "acl_cpp/master/master_udp.hpp"
#endif

namespace acl
{

master_udp::master_udp(void) {}

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

static bool __has_called = false;

void master_udp::run(int argc, char** argv)
{
	// ���� acl ����������� UDP ������ģ��ӿ�
	acl_udp_server_main(argc, argv, service_main,
		ACL_MASTER_SERVER_CTX, this,
		ACL_APP_CTL_THREAD_INIT_CTX, this,
		ACL_MASTER_SERVER_PRE_INIT, service_pre_jail,
		ACL_MASTER_SERVER_POST_INIT, service_init,
		ACL_MASTER_SERVER_EXIT, service_exit,
		ACL_MASTER_SERVER_THREAD_INIT, thread_init,
		ACL_MASTER_SERVER_INT_TABLE, conf_.get_int_cfg(),
		ACL_MASTER_SERVER_STR_TABLE, conf_.get_str_cfg(),
		ACL_MASTER_SERVER_BOOL_TABLE, conf_.get_bool_cfg(),
		ACL_MASTER_SERVER_INT64_TABLE, conf_.get_int64_cfg(),
		0);
}

void master_udp::run_daemon(int argc, char** argv)
{
#ifdef ACL_WINDOWS
	logger_fatal("no support win32 yet!");
#else
	// ÿ������ֻ����һ��ʵ��������
	acl_assert(__has_called == false);
	__has_called = true;
	daemon_mode_ = true;

	run(argc, argv);
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
	acl_assert(__has_called == false);
	__has_called = true;
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

	run(argc, (char**) argv);
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

void master_udp::service_main(void* ctx, ACL_VSTREAM *stream)
{
	master_udp* mu = (master_udp *) ctx;
	acl_assert(mu != NULL);

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
	if (mu->daemon_mode_)
		acl_watchdog_pat();  // ����֪ͨ acl_master ���һ��
#endif
*/
	mu->on_read(ss);
}

void master_udp::service_pre_jail(void* ctx)
{
	master_udp* mu = (master_udp *) ctx;
	acl_assert(mu != NULL);

#ifndef ACL_WINDOWS
	ACL_EVENT* eventp = acl_udp_server_event();
	mu->set_event(eventp);
#endif

	mu->proc_pre_jail();
}

void master_udp::service_init(void* ctx)
{
	master_udp* mu = (master_udp *) ctx;
	acl_assert(mu != NULL);

	mu->proc_inited_ = true;
	mu->proc_on_init();
}

void master_udp::service_exit(void* ctx)
{
	master_udp* mu = (master_udp *) ctx;
	acl_assert(mu != NULL);
	mu->proc_on_exit();
}

void master_udp::thread_init(void* ctx)
{
	master_udp* mu = (master_udp *) ctx;
	acl_assert(mu != NULL);
#ifndef	ACL_WINDOWS
	if (mu->daemon_mode_)
	{
		ACL_VSTREAM** streams = acl_udp_server_streams();
		mu->locker_.lock();
		if (streams != NULL)
		{
			for (int i = 0; streams[i] != NULL; i++)
			{
				socket_stream* ss = NEW socket_stream();
				if (ss->open(streams[i]) == false)
					logger_fatal("open stream error!");
				mu->sstreams_.push_back(ss);
			}
		}
		mu->locker_.unlock();
	}
#endif
	mu->thread_on_init();
}

}  // namespace acl
