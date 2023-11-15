#pragma once



	/// Helper class, used to mimic "enum classes" in C++98 and C++03.
template<typename storageType>
class kdEnum
{
public:

	kdEnum() { }
	kdEnum(storageType v) : m_value(v) { }

	operator storageType() const { return m_value; }

	void operator=(storageType v) { m_value = v; }


protected:

	storageType m_value;
};
