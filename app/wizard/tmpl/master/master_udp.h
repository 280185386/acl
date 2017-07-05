#pragma once

//////////////////////////////////////////////////////////////////////////////
// ����������

extern char *var_cfg_str;
extern acl::master_str_tbl var_conf_str_tab[];

extern int  var_cfg_bool;
extern acl::master_bool_tbl var_conf_bool_tab[];

extern int  var_cfg_int;
extern acl::master_int_tbl var_conf_int_tab[];

extern long long int  var_cfg_int64;
extern acl::master_int64_tbl var_conf_int64_tab[];

//////////////////////////////////////////////////////////////////////////////

//class acl::socket_stream;

class master_service : public acl::master_udp
{
public:
	master_service();
	~master_service();

protected:
	/**
	 * @override
	 * �����յ�һ���ͻ��˷���������ʱ���ô˺���
	 * @param stream {aio_socket_stream*} ���� UDP ������
	 */
	void on_read(acl::socket_stream* stream);

	/**
	 * @override
	 * �������л��û���ݺ���õĻص��������˺���������ʱ������
	 * ��Ȩ��Ϊ��ͨ���޼���
	 */
	void proc_on_init();

	/**
	 * @override
	 * �������˳�ǰ���õĻص�����
	 */
	void proc_on_exit();

	/**
	 * @override
	 * �������յ� SIGHUP �źź�Ļص�����
	 */
	void proc_on_sighup();
};
