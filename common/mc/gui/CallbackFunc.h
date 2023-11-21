#pragma once


template<typename MemberFunc>
struct MemberFuncTraits
{
	typedef MemberFunc MemberFunc;
};

template<class TargetClass>
struct MemberFuncTraits<void (TargetClass::*)()>
{
	typedef TargetClass TargetClass;
};



template<typename Func>
class CallbackFunc
{
public:

	typedef typename MemberFuncTraits<Func>::TargetClass TargetClass;

public:

	CallbackFunc()
		: m_target(0)
		, m_func(0)
	{

	}

	void set(TargetClass* target, Func func) { m_target = target; m_func = func; }

	void invoke() { if (m_target && m_func) (m_target->*m_func)(); }


private:

	TargetClass* m_target;
	Func m_func;
};
