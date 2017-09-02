#pragma once
#include "../acl_cpp_define.hpp"
#include "noncopyable.hpp"

namespace acl
{

// internal functions being used
void*  mbox_create(void);
void   mbox_free(void*, void (*free_fn)(void*));
bool   mbox_send(void*, void*);
void*  mbox_read(void*, int, bool*);
size_t mbox_nsend(void*);
size_t mbox_nread(void*);

template<typename T>
class mbox
{
public:
	mbox(void)
	{
		mbox_ = mbox_create();
	}

	~mbox(void)
	{
		mbox_free(mbox_, mbox_free_fn);
	}

	/**
	 * ������Ϣ����
	 * @param t {T*} �ǿ���Ϣ����
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool push(T* t)
	{
		return mbox_send(mbox_, t);
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
		return (T*) mbox_read(mbox_, timeout, success);
	}

	/**
	 * ͳ�Ƶ�ǰ�Ѿ����͵���Ϣ��
	 * @return {size_t}
	 */
	size_t push_count(void) const
	{
		return mbox_nsend(mbox_);
	}

	/**
	 * ͳ�Ƶ�ǰ�Ѿ����յ�����Ϣ��
	 * @return {size_t}
	 */
	size_t pop_count(void) const
	{
		return mbox_nread(mbox_);
	}

private:
	void* mbox_;

	static void mbox_free_fn(void* o)
	{
		T* t = (T*) o;
		delete t;
	}
};

} // namespace acl
