#pragma once
#include "master_base.hpp"
#include "../stdlib/locker.hpp"

namespace acl {

class ACL_CPP_API master_udp : public master_base
{
public:
	/**
	 * ��ʼ���У����øú�����ָ�÷���������� acl_master ������
	 * ����֮�����У�һ������������״̬
	 * @param argc {int} �� main �д��ݵĵ�һ����������ʾ��������
	 * @param argv {char**} �� main �д��ݵĵڶ�������
	 */
	void run_daemon(int argc, char** argv);

	/**
	 * �ڵ�������ʱ�Ĵ��������û����Ե��ô˺�������һЩ��Ҫ�ĵ��Թ���
	 * @param addrs {const char*} ���������ַ�б���ʽ��IP:PORT, IP:PORT...
	 * @param path {const char*} �����ļ�ȫ·��
	 * @param count {unsigned int} ѭ������Ĵ������ﵽ��ֵ�����Զ����أ�
	 *  ����ֵΪ 0 ���ʾ����һֱѭ���������������������
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool run_alone(const char* addrs, const char* path = NULL,
		unsigned int count = 1);

protected:
	// ���಻��ֱ�ӱ�ʵ����
	master_udp();
	virtual ~master_udp();

	/**
	 * ���麯������ UDP �������ݿɶ�ʱ�ص�����˺���
	 * @param stream {socket_stream*}
	 */
	virtual void on_read(socket_stream* stream) = 0;

	/**
	 * ��ñ��ؼ������׽ӿ������󼯺�
	 * @return {const std::vector<socket_stream*>&}
	 */
	const std::vector<socket_stream*>& get_sstreams() const
	{
		return sstreams_;
	}

	/**
	 * ���̳߳�ʼ��ʱ���鷽����������
	 */
	virtual void thread_on_init(void) {}

private:
	std::vector<socket_stream*> sstreams_;
	locker locker_;

	void close_sstreams(void);
	void run(int argc, char** argv);

private:
	// �����յ�һ���ͻ�������ʱ�ص��˺���
	static void service_main(void*, ACL_VSTREAM*);

	// �������л��û���ݺ���õĻص�����
	static void service_pre_jail(void*);

	// �������л��û���ݺ���õĻص�����
	static void service_init(void*);

	// �������˳�ʱ���õĻص�����
	static void service_exit(void*);

	// ���߳�����ʱ���õĻص�����
	static void thread_init(void*);
};

} // namespace acl
