#pragma once

//////////////////////////////////////////////////////////////////////////////
// ����������

class tcp_service : public acl::master_thread
{
public:
	tcp_service(void);
	~tcp_service(void);

protected:
	// @override
	bool thread_on_read(acl::socket_stream* stream);

	// @override
	bool thread_on_accept(acl::socket_stream* stream);

	// @override
	void thread_on_close(acl::socket_stream* stream);

	// @override
	void thread_on_init(void);

	// @override
	void thread_on_exit(void);

	// @override
	void proc_on_init(void);

	// @override
	void proc_on_exit(void);

	// @override
	bool proc_on_sighup(acl::string&);

private:
	acl::thread_pool* threads_;
};
