#pragma once
#include "../acl_cpp_define.hpp"
#include "noncopyable.hpp"

// just forward declare functions which are in lib_acl.a

struct ACL_MBOX;

extern "C" {
extern ACL_MBOX *acl_mbox_create(void);
extern void   acl_mbox_free(ACL_MBOX *mbox, void (*free_fn)(void*));
extern int    acl_mbox_send(ACL_MBOX *mbox, void *msg);
extern void  *acl_mbox_read(ACL_MBOX *mbox, int timeout, int *success);
extern size_t acl_mbox_nsend(ACL_MBOX *mbox);
extern size_t acl_mbox_nread(ACL_MBOX *mbox);
}

namespace acl
{

template<typename T>
class mbox
{
public:
	mbox(void)
	{
		mbox_ = acl_mbox_create();
	}

	~mbox(void)
	{
		acl_mbox_free(mbox_, mbox_free_fn);
	}

	/**
	 * ������Ϣ����
	 * @param t {T*} �ǿ���Ϣ����
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool push(T* t)
	{
		return acl_mbox_send(mbox_, t) == 0;
	}

	/**
	 * ������Ϣ����
	 * @param timeout {int} ���� 0 ʱ���ö��ȴ���ʱʱ��(���뼶��)������
	 *  ��Զ�ȴ�ֱ��������Ϣ��������
	 * @param success {bool*} �������ڸ���ȷ���������Ƿ�ɹ�
	 * @return {T*} �� NULL ��ʾ����һ����Ϣ����Ϊ NULL ʱ������ͨ��
	 *  success �����ķ���ֵ�������Ƿ�ɹ�
	 */
	T* pop(int timeout = 0, bool* success = NULL)
	{
		int ok;
		void* o = (void*) acl_mbox_read(mbox_, timeout, &ok);
		if (success)
			*success = ok ? true : false;
		return (T*) o;
	}

	/**
	 * ͳ�Ƶ�ǰ�Ѿ����͵���Ϣ��
	 * @return {size_t}
	 */
	size_t push_count(void) const
	{
		return acl_mbox_nsend(mbox_);
	}

	/**
	 * ͳ�Ƶ�ǰ�Ѿ����յ�����Ϣ��
	 * @return {size_t}
	 */
	size_t pop_count(void) const
	{
		return acl_mbox_nread(mbox_);
	}

private:
	ACL_MBOX* mbox_;

	static void mbox_free_fn(void* o)
	{
		T* t = (T*) o;
		delete t;
	}
};

} // namespace acl
