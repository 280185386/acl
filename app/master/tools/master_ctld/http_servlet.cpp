#include "stdafx.h"
#include "action/action.h"
#include "http_servlet.h"

http_servlet::http_servlet(acl::socket_stream* stream, acl::session* session,
	const char* addr)
: acl::HttpServlet(stream, session)
, addr_(addr)
{
}

http_servlet::~http_servlet(void)
{
}

bool http_servlet::replyf(acl::HttpServletRequest& req,
	acl::HttpServletResponse& res, int status, const char* fmt, ...)
{
	acl::string buf;
	va_list ap;
	va_start(ap, fmt);
	buf.vformat(fmt, ap);
	va_end(ap);

	return reply(req, res, status, buf);
}

bool http_servlet::reply(acl::HttpServletRequest&,
	acl::HttpServletResponse& res, int status, const acl::string& buf)
{
	res.setStatus(status)
		.setContentType("text/plain; charset=utf-8")
		.setContentLength(buf.size());

	return res.write(buf, buf.size()) && res.write(NULL, 0);
}

bool http_servlet::reply_json(acl::HttpServletRequest&,
	acl::HttpServletResponse& res, int status, const acl::string& data)
{
	res.setStatus(status)
		.setContentType("text/json; charset=utf-8")
		.setContentLength(data.size());

	return res.write(data, data.size()) && res.write(NULL, 0);
}

bool http_servlet::doError(acl::HttpServletRequest&,
	acl::HttpServletResponse& res)
{
	res.setStatus(400);
	res.setContentType("text/xml; charset=utf-8");

	// ���� http ��Ӧ��
	acl::string buf;
	buf.format("<root error='some error happened!' />\r\n");
	res.write(buf);
	res.write(NULL, 0);
	return false;
}

bool http_servlet::doOther(acl::HttpServletRequest&,
	acl::HttpServletResponse& res, const char* method)
{
	res.setStatus(400);
	res.setContentType("text/xml; charset=utf-8");
	// ���� http ��Ӧ��
	acl::string buf;
	buf.format("<root error='unkown request method %s' />\r\n", method);
	res.write(buf);
	res.write(NULL, 0);
	return false;
}

bool http_servlet::doGet(acl::HttpServletRequest& req,
	acl::HttpServletResponse& res)
{
	return doPost(req, res);
}

bool http_servlet::doPost(acl::HttpServletRequest& req,
	acl::HttpServletResponse& res)
{
	// �����Ҫ http session ���ƣ��������ע�ͣ�����Ҫ��֤
	// �� master_service.cpp �ĺ��� thread_on_read �����õ�
	// memcached ������������
	/*
	const char* sid = req.getSession().getAttribute("sid");
	if (*sid == 0)
		req.getSession().setAttribute("sid", "xxxxxx");
	sid = req.getSession().getAttribute("sid");
	*/

	// �����Ҫȡ������� cookie �������ע��
	/*
	
	*/

	res.setContentType("text/xml; charset=utf-8")	// ������Ӧ�ַ���
		.setKeepAlive(req.isKeepAlive())	// �����Ƿ񱣳ֳ�����
		.setContentEncoding(true)		// �Զ�֧��ѹ������
		.setChunkedTransferEncoding(false);	// chunk ���䷽ʽ

	const char* cmd = req.getParameter("cmd");
	if (cmd == NULL || *cmd == 0)
	{
		logger_error("cmd not found");
		return replyf(req, res, 400, "%s", "no cmd");
	}

#define EQ	!strcasecmp

	base_action* action;

	if (EQ(cmd, "list"))
		action = new list_action;
	else if (EQ(cmd, "stat"))
		action = new stat_action;
	else if (EQ(cmd, "start"))
		action = new start_action;
	else if (EQ(cmd, "stop"))
		action = new stop_action;
	else if (EQ(cmd, "kill"))
		action = new kill_action;
	else if (EQ(cmd, "reload"))
		action = new reload_action;
	else if (EQ(cmd, "restart"))
		action = new restart_action;
	else
	{
		logger_error("invalid cmd=%s", cmd);
		return replyf(req, res, 400, "invalid cmd=%s", cmd);
	}

	action->prepare(addr_, req, res);

	acl::string buf;
	int status = action->run(buf);
	delete action;
	return reply_json(req, res, status, buf);
}
