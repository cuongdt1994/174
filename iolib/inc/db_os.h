#ifndef __GNET_DB_OCTETS_HPP
#define __GNET_DB_OCTETS_HPP

#include <vector>
#include <mutex>
#include <cstring>
#include <string>
#include <unordered_map>
#include <climits>

namespace GDB
{
    class DBStream {
    public:
        DBStream() = default;

        DBStream(const std::vector<unsigned char>& data) : data_(data) {}

        DBStream(std::vector<unsigned char>&& data) : data_(std::move(data)) {}

        DBStream(const DBStream& other) : data_(other.data_) {}

        DBStream(DBStream&& other) : data_(std::move(other.data_)) {}

        DBStream(const void* buf, size_t size) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (buf && size > 0 && size < data_.max_size()) {
                const unsigned char* ptr = reinterpret_cast<const unsigned char*>(buf);
                data_.insert(data_.end(), ptr, ptr + size);
            }
        }

		~DBStream()
		{
			std::lock_guard<std::mutex> lock(mutex_);
			data_.clear();
		}

		void clear()
		{
			std::lock_guard<std::mutex> lock(mutex_);
			data_.clear();
		}

        void push_back(unsigned char value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() == data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.push_back(value);
        }

        void push_back(const void* buf, const size_t size) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + size > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            if (buf && size) {
                const unsigned char* ptr = reinterpret_cast<const unsigned char*>(buf);
                data_.insert(data_.end(), ptr, ptr + size);
            }
        }

        unsigned char pop_back() {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.empty()) {
                throw std::out_of_range("DBStream is empty");
            }
            unsigned char value = data_.back();
            data_.pop_back();
            return value;
        }

        void pop_back(void* buf, size_t size) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (size > data_.size()) {
                throw std::out_of_range("DBStream size out of range");
            }
            if (buf && size) {
                unsigned char* ptr = reinterpret_cast<unsigned char*>(buf);
                std::memcpy(ptr, data_.data() + data_.size() - size, size);
            }
            data_.resize(data_.size() - size);
        }

        void replace(size_t pos, const void* buf, size_t size) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (pos + size > data_.size()) {
                throw std::out_of_range("DBStream position or size out of range");
            }
            const unsigned char* ptr = reinterpret_cast<const unsigned char*>(buf);
            std::memcpy(data_.data() + pos, ptr, size);
        }

        void replace(const void* buf, size_t size) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (size > data_.max_size()) {
                throw std::length_error("DBStream size exceeds maximum size");
            }
            const unsigned char* ptr = reinterpret_cast<const unsigned char*>(buf);
            data_.assign(ptr, ptr + size);
        }

        void resize(size_t size) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (size > data_.max_size()) {
                throw std::length_error("DBStream size exceeds maximum size");
            }
            data_.resize(size);
        }

        size_t size() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return data_.size();
        }

        bool empty() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return data_.empty();
        }

        const unsigned char* data() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return data_.data();
        }

        unsigned char* data() {
            std::lock_guard<std::mutex> lock(mutex_);
            return data_.data();
        }

        const void* begin() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return data_.data();
        }

        void* begin() {
            std::lock_guard<std::mutex> lock(mutex_);
            return data_.data();
        }

        const char* c_str() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return reinterpret_cast<const char*>(data_.data());
        }

        DBStream& operator=(const DBStream& other) {
            if (this != &other) {
                std::lock_guard<std::mutex> lock(mutex_);
                data_ = other.data_;
            }
            return *this;
        }

        DBStream& operator=(DBStream&& other) {
            if (this != &other) {
                std::lock_guard<std::mutex> lock(mutex_);
                data_ = std::move(other.data_);
            }
            return *this;
        }

        const unsigned char& operator[](size_t index) const {
            std::lock_guard<std::mutex> lock(mutex_);
            if (index > data_.size()) {
                throw std::length_error("DBStream size exceeds index > os.size");
            }
            return data_[index];
        }

        unsigned char& operator[](size_t index) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (index > data_.size()) {
                throw std::length_error("DBStream size exceeds index > os.size");
            }
            return data_[index];
        }

        DBStream& operator+=(const DBStream& other) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + other.size() > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), other.data_.begin(), other.data_.end());
            return *this;
        }

        // operator <<

        DBStream& operator<<(char value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + sizeof(value) > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), reinterpret_cast<const unsigned char*>(&value), reinterpret_cast<const unsigned char*>(&value) + sizeof(value));
            return *this;
        }

        DBStream& operator<<(unsigned char value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + sizeof(value) > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.push_back(value);
            return *this;
        }

        DBStream& operator<<(short value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + sizeof(value) > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), reinterpret_cast<const unsigned char*>(&value), reinterpret_cast<const unsigned char*>(&value) + sizeof(value));
            return *this;
        }

        DBStream& operator<<(unsigned short value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + sizeof(value) > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), reinterpret_cast<const unsigned char*>(&value), reinterpret_cast<const unsigned char*>(&value) + sizeof(value));
            return *this;
        }

        DBStream& operator<<(int value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + sizeof(value) > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), reinterpret_cast<const unsigned char*>(&value), reinterpret_cast<const unsigned char*>(&value) + sizeof(value));
            return *this;
        }

        DBStream& operator<<(unsigned int value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + sizeof(value) > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), reinterpret_cast<const unsigned char*>(&value), reinterpret_cast<const unsigned char*>(&value) + sizeof(value));
            return *this;
        }

        DBStream& operator<<(long value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + sizeof(value) > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), reinterpret_cast<const unsigned char*>(&value), reinterpret_cast<const unsigned char*>(&value) + sizeof(value));
            return *this;
        }

        DBStream& operator<<(unsigned long value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + sizeof(value) > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), reinterpret_cast<const unsigned char*>(&value), reinterpret_cast<const unsigned char*>(&value) + sizeof(value));
            return *this;
        }

        DBStream& operator<<(long long value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + sizeof(value) > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), reinterpret_cast<const unsigned char*>(&value), reinterpret_cast<const unsigned char*>(&value) + sizeof(value));
            return *this;
        }

        DBStream& operator<<(unsigned long long value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + sizeof(value) > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), reinterpret_cast<const unsigned char*>(&value), reinterpret_cast<const unsigned char*>(&value) + sizeof(value));
            return *this;
        }

        DBStream& operator<<(const char* value) {
            std::lock_guard<std::mutex> lock(mutex_);
            size_t size = std::strlen(value);
            if (data_.size() + size + 1 > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), reinterpret_cast<const unsigned char*>(value), reinterpret_cast<const unsigned char*>(value) + size);
            data_.push_back('\0');
            return *this;
        }

        DBStream& operator<<(const std::string& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + value.size() + 1 > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), reinterpret_cast<const unsigned char*>(value.c_str()), reinterpret_cast<const unsigned char*>(value.c_str()) + value.size());
            data_.push_back('\0');
            return *this;
        }

        DBStream& operator<<(const std::u16string& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + value.size() * sizeof(char16_t) + sizeof(char16_t) > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), reinterpret_cast<const unsigned char*>(value.c_str()), reinterpret_cast<const unsigned char*>(value.c_str()) + value.size() * sizeof(char16_t));
            data_.push_back('\0');
            return *this;
        }

        DBStream& operator<<(const std::wstring& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + value.size() * sizeof(wchar_t) + sizeof(wchar_t) > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), reinterpret_cast<const unsigned char*>(value.c_str()), reinterpret_cast<const unsigned char*>(value.c_str()) + value.size() * sizeof(wchar_t));
            data_.push_back('\0');
            return *this;
        }

        DBStream& operator<<(const DBStream& other) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() + other.data_.size() > data_.max_size()) {
                throw std::length_error("DBStream capacity exceeded");
            }
            data_.insert(data_.end(), other.data_.begin(), other.data_.end());
            return *this;
        }

        // operator >>

        DBStream& operator>>(char& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() < sizeof(value)) {
                throw std::out_of_range("DBStream is empty or contains insufficient data");
            }
            std::memcpy(&value, data_.data(), sizeof(value));
            data_.erase(data_.begin(), data_.begin() + sizeof(value));
            return *this;
        }

        DBStream& operator>>(unsigned char& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() < sizeof(value)) {
                throw std::out_of_range("DBStream is empty or contains insufficient data");
            }
            std::memcpy(&value, data_.data(), sizeof(value));
            data_.erase(data_.begin(), data_.begin() + sizeof(value));
            return *this;
        }

        DBStream& operator>>(short& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() < sizeof(value)) {
                throw std::out_of_range("DBStream is empty or contains insufficient data");
            }
            std::memcpy(&value, data_.data(), sizeof(value));
            data_.erase(data_.begin(), data_.begin() + sizeof(value));
            return *this;
        }

        DBStream& operator>>(unsigned short& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() < sizeof(value)) {
                throw std::out_of_range("DBStream is empty or contains insufficient data");
            }
            std::memcpy(&value, data_.data(), sizeof(value));
            data_.erase(data_.begin(), data_.begin() + sizeof(value));
            return *this;
        }

        DBStream& operator>>(int& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() < sizeof(value)) {
                throw std::out_of_range("DBStream is empty or contains insufficient data");
            }
            std::memcpy(&value, data_.data(), sizeof(value));
            data_.erase(data_.begin(), data_.begin() + sizeof(value));
            return *this;
        }

        DBStream& operator>>(unsigned int& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() < sizeof(value)) {
                throw std::out_of_range("DBStream is empty or contains insufficient data");
            }
            std::memcpy(&value, data_.data(), sizeof(value));
            data_.erase(data_.begin(), data_.begin() + sizeof(value));
            return *this;
        }

        DBStream& operator>>(long& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() < sizeof(value)) {
                throw std::out_of_range("DBStream is empty or contains insufficient data");
            }
            std::memcpy(&value, data_.data(), sizeof(value));
            data_.erase(data_.begin(), data_.begin() + sizeof(value));
            return *this;
        }

        DBStream& operator>>(unsigned long& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() < sizeof(value)) {
                throw std::out_of_range("DBStream is empty or contains insufficient data");
            }
            std::memcpy(&value, data_.data(), sizeof(value));
            data_.erase(data_.begin(), data_.begin() + sizeof(value));
            return *this;
        }

        DBStream& operator>>(long long& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() < sizeof(value)) {
                throw std::out_of_range("DBStream is empty or contains insufficient data");
            }
            std::memcpy(&value, data_.data(), sizeof(value));
            data_.erase(data_.begin(), data_.begin() + sizeof(value));
            return *this;
        }

        DBStream& operator>>(unsigned long long& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.size() < sizeof(value)) {
                throw std::out_of_range("DBStream is empty or contains insufficient data");
            }
            std::memcpy(&value, data_.data(), sizeof(value));
            data_.erase(data_.begin(), data_.begin() + sizeof(value));
            return *this;
        }

        DBStream& operator>>(char* value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.empty()) {
                throw std::out_of_range("DBStream is empty");
            }
            if (data_.size() > std::strlen(value)) {
                throw std::length_error("Buffer size is less than DBStream size");
            }
            std::memcpy(value, data_.data(), data_.size());
            value[data_.size()] = '\0';
            data_.clear();
            return *this;
        }

        DBStream& operator>>(std::string& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.empty()) {
                throw std::out_of_range("DBStream is empty");
            }
            value.assign(reinterpret_cast<const char*>(data_.data()), data_.size() - 1);
            data_.clear();
            return *this;
        }

        DBStream& operator>>(std::u16string& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.empty()) {
                throw std::out_of_range("DBStream is empty");
            }
            size_t size = (data_.size() - 1) / sizeof(char16_t);
            if (size * sizeof(char16_t) > value.max_size()) {
                throw std::length_error("u16string size exceeds maximum size");
            }
            value.assign(reinterpret_cast<const char16_t*>(data_.data()), size);
            data_.erase(data_.begin(), data_.begin() + size * sizeof(char16_t) + sizeof(char16_t));
            return *this;
        }

        DBStream& operator>>(std::wstring& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_.empty()) {
                throw std::out_of_range("DBStream is empty");
            }
            size_t size = (data_.size() - 1) / sizeof(wchar_t);
            if (size * sizeof(wchar_t) > value.max_size()) {
                throw std::length_error("wstring size exceeds maximum size");
            }
            value.assign(reinterpret_cast<const wchar_t*>(data_.data()), size);
            data_.erase(data_.begin(), data_.begin() + size * sizeof(wchar_t) + sizeof(wchar_t));
            return *this;
        }

    private:
        std::vector<unsigned char> data_;
        mutable std::mutex mutex_;
    };


	inline bool DBSSet( DBStream & os , const void * buf, const size_t size )
	{
		bool res = false;
		try
		{
			os.clear();
			if (buf && size > 0 && size < INT_MAX)
			{
				os.push_back(buf, size);
				res = true;
			}
		}
		catch ( ... )
		{
			printf("DBSSet: FAILED!!! \n");
		}
		return res;
	}

	inline bool DBSGet( const DBStream & os , void * buf, size_t size )
	{
		bool res = false;
		try
		{
			if (buf && size > 0 && size < INT_MAX && os.size() > 0 && os.size() < INT_MAX)
			{
				memset(buf, 0x00, size);
				if ( size > os.size() )
				{
					size = os.size();
				}
				memcpy(buf, os.begin(), size );
			}
		}
		catch ( ... )
		{
			printf("DBSGet: FAILED!!! \n");
		}
		return res;
	}

};

// hashmap DBStream 
namespace std
{
    template<>
    struct hash<GDB::DBStream>
    {
        size_t operator() (const GDB::DBStream& __os) const
        {
            const char* __p = (const char*)__os.begin();
            unsigned long __h = 0;
            for (int len = __os.size(); len-- > 0; )  __h = 5 * __h + *__p++;
            return size_t(__h);
        }
    };
};


#endif

