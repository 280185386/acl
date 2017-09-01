#pragma once
#include "../acl_cpp_define.hpp"
#include "noncopyable.hpp"

struct ACL_MBOX;

namespace acl
{

//class ACL_CPP_API mbox : public noncopyable
template<typename T>
class mbox
{
public:
	mbox(void);
	~mbox(void) {}

	/**
	 * ������Ϣ����
	 * @param t {T*} �ǿ���Ϣ����
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool push(T* t)
	{
		return push_one((void*) t);
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
		return (T*) pop_one(timeout, success);
	}

	/**
	 * ͳ�Ƶ�ǰ�Ѿ����͵���Ϣ��
	 * @return {size_t}
	 */
	size_t push_count(void) const;

	/**
	 * ͳ�Ƶ�ǰ�Ѿ����յ�����Ϣ��
	 * @return {size_t}
	 */
	size_t pop_count(void) const;

private:
	ACL_MBOX* mbox_;

	bool  push_one(void* o);
	void* pop_one(int timeout, bool* success);
};

} // namespace acl
