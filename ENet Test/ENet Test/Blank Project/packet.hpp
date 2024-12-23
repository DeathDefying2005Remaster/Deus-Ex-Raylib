#include <cstdint>
#include <cstddef>
#include <vector>
using namespace std;

class Packet
{
	private:

	std::vector<std::byte> m_data;

	public:

	Packet()
	{

	}
	~Packet()
	{

	}

	size_t size()
	{
		return m_data.size();
	}

	std::vector<std::byte> data()
	{
		return m_data;
	}

	void Append(const void* data, std::size_t sizeInBytes)
	{
		if (data && (sizeInBytes > 0))
		{
			const auto* begin = reinterpret_cast<const std::byte*>(data);
			const auto* end = begin + sizeInBytes;
			m_data.insert(m_data.end(), begin, end);
		}
	}
};