#pragma once
using namespace std;
#include "..\common.h"
#include "vector_itaretors.h"
#include "slice.h"
#define MINIMAL_VECTOR_SIZE 8
#define VECTOR_SIZE_UPSCALE 1.5

template <typename T>
class template_vector_
{
private:
	struct slice_object
	{
	private:
		static I32 calc_index(const I32 index, const I32 stride, const I32 step) {
			return index % (stride + step);
		};
	private:
		I32 m_begin;
		I32 m_end;
		I32 m_step;
		template_vector_<T>* m_source = nullptr;

	public:
		I32 begin()const { return m_begin; };

		I32 end()const { return m_end; };

		I32 step()const { return m_step; };

		const template_vector_<T>& source()const { return *m_source; };
		
		template_vector_<T>& source(){ return *m_source; };

		bool shift_begin(const I32 amount)
		{
			const I32 value = begin() + amount;
			if (value > source().filling()) return false;
			if (value < 0) return false;
			m_begin = value;
			return true;
		}

		bool shift_end(const I32 amount) {
			const I32 value = end() + amount;
			if (value > source().filling()) return false;
			if (value < 0) return false;
			m_end = value;
			return true;
		}

		I32 length() const {
			const I32 total = std::abs(end() - begin());
			return std::abs(total / step() + total % step());
		}

		I32 source_index(const I32 index)const {
			return begin() + index * step();
		}

		I32 slice_index(const I32 index)const {
			return (index - begin()) / step();
		}

		slice_object(slice_object& other) {
			m_begin  =   other. m_begin ;
			m_end    =   other. m_end   ;
			m_step   =   other. m_step  ;
			m_source = *(other.m_source);
		}

		slice_object(const slice& _slice, template_vector_<T>& source) {
			m_begin = calc_index(_slice.begin(), source.filling(), _slice.step());
			m_end   = calc_index(_slice.end(),   source.filling(), _slice.step());
			m_step  = _slice.step();
			if (begin() < 0) throw std::runtime_error("Unable to take slice with begin border of value " + std::to_string(m_begin) +
													  ", while total length is" + std::to_string(source.filling()));
			
			if (end() < 0) throw std::runtime_error("Unable to take slice with end border of value " + std::to_string(m_end) +
												    ", while total length is" + std::to_string(source.filling()));
			
			if (end() == begin()) throw std::runtime_error("Empty slice!!!");
			if ((begin() > end()) && (m_step > 0)) std::swap(m_begin, m_end);
			m_source = *source;
		}

		~slice_object() {
			m_source = nullptr;
		};
	};

	T* alloc(const I32 cap)
	{
		return (T*)(malloc(cap * sizeof(T)));
	}
	void dealloc() 
	{
		if (m_data != nullptr) free(m_data);
	}
	void upscale()
	{
		if (filling() != capacity())return;
		resize(I32(m_capacity * VECTOR_SIZE_UPSCALE));
	}
	slice_object* m_slice = nullptr;
	T*            m_data;
	I32           m_capacity;
	I32           m_filling;
	bool          m_is_silce = false;
protected:
	void exchange_data(template_vector_<T>&& other)noexcept
	{
		if (*this == other) return;
		if (is_slice()) 
		{
			apply(other, [](const T& v) {return v; });
			return;
		}
		dealloc();
		if (other.is_slice()) 
		{
			m_data     = alloc(other.capacity());
			m_capacity = other.capacity();
			m_filling  = other.filling();
			apply(other, [](const T& v) {return v;});
			return;
		};
		m_data     = EXCHANGE(other.m_data    , nullptr);
		m_capacity = EXCHANGE(other.m_capacity, -1);
		m_filling  = EXCHANGE(other.m_filling , -1);
	}

	void reassign_data(const template_vector_<T>& other)
	{
		if (*this == other)
			return;

		if (capacity() < other.filling())           // resource in *this cannot be reused
		{
			dealloc();                                      // release resource in *this
			m_data = alloc(other.capacity());
		}

		m_capacity = other.capacity();
		m_filling = other.filling();

		if (other.is_slice())
			apply(other, [](const T& v) {return v; });
		else
			memcpy(m_data, other.m_data, filling() * sizeof(T));
	}
public:
	static T reduce(const template_vector_<T>& vector, std::function<T(const T&, const T&)> reduce_f, T init_value = T{ 0 });

	static bool all(const template_vector_<T>& vector, std::function<bool(const T&)> condition_f);
	
	static bool any(const template_vector_<T>& vector, std::function<bool(const T&)> condition_f);

	static  template_vector_<T> filter(const template_vector_<T>& vector, std::function<bool(const T&)> condition_f);

	template<typename T1>
	static map_values<T, T1>map(const template_vector_<T>& left, std::function<T1(const T&)> map_f);

	template<typename T1>
	static zip_values<T, T1>zip(const template_vector_<T>& left, const template_vector_<T1>& right);

	template<typename T1>
	static zip_values<T, T1>zip(const template_vector_<T>& left, const T1& right);

	template<typename T1>
	static zip_values<T, T1>zip(const T& left, const template_vector_<T1>& right);

	template<typename T1>
	static combine_values<T, T1>combine(const template_vector_<T>& left, const template_vector_<T>& right, std::function<T1(const T&, const T&)> combine_f);

	template<typename T1>
	static combine_values<T, T1>combine(const T& left, const template_vector_<T>& right, std::function<T1(const T&, const T&)> combine_f);
	
	template<typename T1>
	static combine_values<T, T1>combine(const template_vector_<T>& left, const T& right, std::function<T1(const T&, const T&)> combine_f);

	T reduce(std::function<T(const T&, const T&)> reduce_f, T init_value = T{ 0 })const
	{
		vector_values<T> values = (*this).values();
		return template_vector_::reduce(values, reduce_f, init_value);
	}

	bool all(std::function<bool(const T&)> condition_f) 
	{
		return template_vector_::all((*this), condition_f);
	}

	bool any(std::function<bool(const T&)> condition_f)
	{
		return template_vector_::any((*this), condition_f);
	}

	void fill(std::function<T(const I32)>  fill_f) {
		for (I32 index : indices()) m_data[index] = fill_f(index);
	}

	void apply(const combine_values<T, T> combiner) 
	{
		I32 index = 0;
		for (const T value : combiner)
		{
			if (index == filling()) break;
			unchecked_access(index++) = value;
		}
	}

	void apply(const template_vector_<T>& vector, std::function<T(const T&)>  apply_f) {
		I32 index = 0;
		for(const T& value: vector.values())
		{
			if (index == filling()) break;
			unchecked_access(index++) = apply_f(value);
		}
	}

	void apply(std::function<T(const T&)>  apply_f) 
	{
		apply((*this), apply_f);
	}

	void apply_enumerate(const template_vector_<T>& vector, std::function<T(const I32, const T&)>  apply_f) {
		I32 index = 0;
		for (const T value : vector.values())
		{
			if (index == filling()) break;
			unchecked_access(index) = apply_f(index, value);
			index++;
		}
	}

	void apply_enumerate(std::function<T(const I32, const T&)>  apply_f)
	{
		apply_enumerate((*this), apply_f);
	}

	template_vector_<T> filter(std::function<bool(const T&)> condition_f)
	{
		return template_vector_::filter((*this), condition_f);
	}

	vector_indices indices() const{
		if(is_slice())
			return vector_indices(m_slice->begin(), m_slice->end(), m_slice->step());
		return vector_indices(0, this->filling(), 1);
	};

	vector_values<T> values() const{
		return vector_values<T>(m_data, indices());
	};

	template<typename T1>
	map_values<T, T1> map(std::function<T1(const T&)> map_f) const {
		return template_vector_::map((*this), map_f);
	};

	template<typename T1>
	zip_values<T, T1> zip(const template_vector_<T1>& other) const {
		return template_vector_::zip((*this), other);
	};

	template<typename T1>
	combine_values<T, T1> combine(const T& right, std::function<T1(const T&, const T&)> combine_f) const {
		return template_vector_::combine((*this), right, combine_f);
	};

	template<typename T1>
	combine_values<T, T1> combine(const template_vector_<T>& right, std::function<T1(const T&, const T&)> combine_f) const {
		return template_vector_::combine((*this), right, combine_f);
	};

	I32 capacity()const { return m_capacity; }

	I32 filling()const { return m_filling; }

	I32 size()const { return m_filling; }

	bool is_empty()const { return filling() == 0; }

	bool is_slice()const { return m_slice != nullptr; }

	const T& unchecked_access(const I32 index) const { return m_data[is_slice() ? m_slice->source_index(index) : index]; }

	T& unchecked_access(const I32 index) { return m_data[is_slice() ? m_slice->source_index(index) : index]; }

	bool in_range(const I32 index)const { return (index >= 0) && (index < filling()); }

	void resize(const I32 new_size)
	{
		if (capacity() == new_size) return;
		m_capacity = new_size;
		m_filling  = capacity() > filling() ? filling() : capacity();
		T* _new_values = alloc(new_size);
		std::memcpy(_new_values, m_data, filling() * sizeof(T));
		dealloc();
		m_data = std::move(_new_values);
	}

	template_vector_<T>& clear() 
	{
		m_filling = 0; 
		return (*this);
	}

	I32 index_of(const T& value)const
	{
		I32 index = 0;
		for (const T& val: values())
		{
			index++;
			if (val == value)break;
		}
		return index == filling() ? -1 : is_slice() ? m_slice->slice_index(index - 1) : index - 1;
	}

	bool contans(const T& value)const
	{
		return index_of(value) != -1;
	}

	template_vector_<T>& remove(const T& value)
	{
		const I32 index = index_of(value);
		if (index == -1) return (*this);
		return remove_at(index);
	}

	template_vector_<T>& push_back(const T& value)
	{
		if (is_slice()) {
			m_slice->source().insert(m_slice->source_index(filling()), value);
			m_filling++;
			return (*this);
		}
		upscale();
		m_data[m_filling++] = value;
		return (*this);
	}

	template_vector_<T>& push_back(T&& value)
	{
		if (is_slice()) {
			m_slice->source().insert(m_slice->source_index(filling()), std::move(value));
			m_filling++;
			return (*this);
		}
		upscale();
		m_data[m_filling++] = std::move(value);
		return (*this);
	}
	
	template_vector_<T>& remove_at(const I32 index) 
	{
		if (is_slice())
		{
			m_slice->source().remove_at(m_slice->source_index(index));
			m_filling--;
			m_slice->shift_end(-1);
			return (*this);
		}
		if (!in_range(index))return (*this);
		std::memcpy(&m_data[index], &m_data[index + 1], I32(filling() - index) * sizeof(T));
		m_filling--;
		return (*this);
	}
		
	template_vector_<T>& insert(const I32 index, const T& value)
	{
		if (is_slice())
		{
			m_slice->source().insert(m_slice->source_index(index), value);
			m_slice->shift_end(1);
			m_filling++;
			return (*this);
		}
		if (index < 0) return insert(0, value);
		if (index > filling()) return push_back(value);
		upscale();
		std::memcpy(&m_data[index + 1], &m_data[index], I32(filling() - index) * sizeof(T));
		m_data[index] = std::move(value);
		m_filling++;
		return (*this);
	}

	template_vector_<T>& insert(const I32 index, T&& value)
	{
		if (is_slice()) 
		{
			m_slice->source().insert(m_slice->source_index(index), std::move(value));
			m_slice->shift_end(1);
			m_filling++;
			return (*this);
		}
		if (index < 0) return insert(0, value);
		if (index > filling()) return push_back(value);
		upscale();
		std::memcpy(&m_data[index + 1], &m_data[index], I32(filling() - index) * sizeof(T));
		m_data[index] = std::move(value);
		m_filling++;
		return (*this);
	}

	template_vector_<T>const& operator&()const { return (*this); }
	
	template_vector_<T>const* operator*()const { return this; }
	
	template_vector_<T>& operator&(){ return (*this); }
	
	template_vector_<T>* operator*(){ return this; }

	T* data() { return m_data; }
	
	const T* data() const { return m_data; }
	
	template_vector_<T>& operator=(template_vector_<T>const& other)
	{
		reassign_data(other);
		return (*this);
	}
	
	template_vector_<T>& operator=(template_vector_<T>&& other) noexcept
	{
		exchange_data(std::move(other));
		return (*this);
	}
	
	T& operator[](const I32 index)
	{
		if (!in_range(index))
			throw new std::runtime_error(
				"template_vector_ :: trying to access index: {" +
				std::to_string(index) + "}, while cap is {" +
				std::to_string(filling()) + "}");
		return unchecked_access(index);
	}
	
	const T& operator[](const I32 index)const
	{
		if (!in_range(index))
			throw new std::runtime_error(
				"template_vector_ :: trying to access index: {" +
				std::to_string(index) + "}, while cap is {" +
				std::to_string(filling()) + "}");
		return unchecked_access(index);
	}
	
	template_vector_<T> operator[](const slice& indices)
	{
		return template_vector_(indices, *this);
	}
	
	const template_vector_<T> operator[](const slice& indices)const
	{
		return template_vector_(indices, *this);
	}

	template_vector_<T>(const template_vector_<T>& other)
	{
		m_data = alloc(other.capacity());
		reassign_data(other);
	};
	
	template_vector_(template_vector_&& other)
	{
		exchange_data(other);
	};
	
	template_vector_(const I32 cap )
	{
		m_filling  = cap;
		m_capacity = (I32)(cap * VECTOR_SIZE_UPSCALE);
		m_data     = alloc(m_capacity);
	};

	template_vector_()
	{
		m_filling  = 0;
		m_capacity = MINIMAL_VECTOR_SIZE;
		m_data     = alloc(m_capacity);
	};

	template_vector_(const initializer_list<T>& values) :template_vector_()
	{
		for (auto v : values) push_back(v);
	};

	// template< typename... Args>
	// template_vector_(T val, Args... values):template_vector_()
	// {
	// 	push_back(values...);
	// };

	template_vector_(const vector_values<T>& values):template_vector_(){
		for (const T& value : values) push_back(value);
	};

	template<typename T1, typename T2>
	template_vector_(const map_values<T1, T2>& values) : template_vector_<T2>(){
		for (const T2 value : values) push_back(value);
	};

	template<typename T1, typename T2>
	template_vector_(const combine_values<T1, T2>& values) : template_vector_<T2>(){
		for (const T2 value : values) push_back(value);
	};
	
	~template_vector_(){
		if (is_slice())
		{
			delete m_slice;
			return;
		};
		dealloc();
	}
	
	template <typename U>
	friend std::ostream& operator<<(std::ostream& steram, const template_vector_<U>& pair);

	template<typename U>
	friend bool operator == (const template_vector_<U>& lhs, const template_vector_<U>& rhs);

protected:
	template_vector_(const slice& slice, template_vector_& source){
		m_slice    = new slice_object(slice, source.is_slice() ? source.m_slice->source() : source);
		m_filling  = m_slice->length();
		m_capacity = source.capacity();
		m_data     = m_slice->source().m_data;
	};
};

template <typename U>
inline std::ostream& operator<<(std::ostream& steram, const template_vector_<U>& vector)
{
	steram << "{ ";
	for (const I32& index: vector.indices()) steram << vector.unchecked_access(index) << (index != vector.filling() - 1 ? ", " : "");
	steram << " }";
	return steram;
}

template<typename T>
inline T template_vector_<T>::reduce(const template_vector_<T>& vector, std::function<T(const T&, const T&)> reduce_f, T init_value)
{
	for (const T& value: vector.values()) init_value = reduce_f(init_value, value);
	return init_value;
}

template<typename T>
static bool template_vector_<T>::all(const template_vector_<T>& vector, std::function<bool(const T&)> condition_f)
{
	for (const T& item : vector.values()) if (!condition_f(item)) return false;
	return true;
}

template<typename T>
static bool template_vector_<T>::any(const template_vector_<T>& vector, std::function<bool(const T&)> condition_f)
{
	for (const T& item : vector.values()) if (condition_f(item)) return true;
	return false;
}

template<typename T>
inline  template_vector_<T>  template_vector_<T>::filter(const template_vector_<T>& vector, std::function<bool(const T&)> condition_f)
{	
	template_vector_<T> filtered_vector;
	for (const T& item : vector.values()) if (condition_f(item)) filtered_vector.push_back(item);
	return filtered_vector;
}

template<typename T>
template<typename T1>
inline zip_values<T, T1> template_vector_<T>::zip(const template_vector_<T>& left, const template_vector_<T1>& right)
{
	return zip_values<T, T1>(left.values(), right.values());
}

template<typename T>
template<typename T1>
inline zip_values<T, T1> template_vector_<T>::zip(const T& left, const template_vector_<T1>& right)
{
	return zip_values<T, T1>(left, right.values());
}

template<typename T>
template<typename T1>
inline zip_values<T, T1> template_vector_<T>::zip(const template_vector_<T>& left, const T1& right)
{
	return zip_values<T, T1>(left.values(), right);
}

template<typename T>
template<typename T1>
inline map_values<T, T1> template_vector_<T>::map(const template_vector_<T>& left, std::function<T1(const T&)> map_f)
{
	return map_values<T, T1>(left.values(), map_f);
}

template<typename T>
template<typename T1>
combine_values<T, T1>template_vector_<T>::combine(const template_vector_<T>& left, 
												  const template_vector_<T>& right,
												  std::function<T1(const T&, const T&)> combine_f)
{
	return combine_values<T, T1>(zip(left, right), combine_f);
}

template<typename T>
template<typename T1>
combine_values<T, T1>template_vector_<T>::combine(const T& left, const template_vector_<T>& right,
												  std::function<T1(const T&, const T&)> combine_f)
{
	return combine_values<T, T1>(zip(left, right), combine_f);
}

template<typename T>
template<typename T1>
combine_values<T, T1>template_vector_<T>::combine(const template_vector_<T>& left, 
												  const T& right, std::function<T1(const T&, const T&)> combine_f)
{
	return combine_values<T, T1>(zip(left, right), combine_f);
}
template<typename U>
bool operator == (const template_vector_<U>& lhs, const template_vector_<U>& rhs) 
{
	return (lhs.filling() == rhs.filling()) && (lhs.m_data == rhs.m_data);
}


void template_vector_test();

void template_vector_test() 
{
	auto first  = template_vector_<I32>();
	first.push_back(11).push_back(22).push_back(33).push_back(44).push_back(55);
	
	map_values<I32, double> int_to_double = first.map<double>([](const I32& i) { return std::sqrt(i); });

	for (auto const a: int_to_double) std::cout << "ccc: " << a << "\n";

	auto second = template_vector_<double>(int_to_double);
	
	auto copy_first = template_vector_<double>(int_to_double);

	copy_first.push_back(11).
			   push_back(22).
			   push_back(33).
			   push_back(55);

	auto copy_of_first = template_vector_<double>(second);

	auto zip_vals = zip_values<double, double>(copy_of_first.values(), copy_of_first.values());

	auto diff = [](const double& a, const double& b) { return a - b; };
	auto summ = [](const double& a, const double& b) { return a + b; };

	// combine_values<double, double> combine_vals = combine_values<double, double>(zip_values<double, double>(copy_of_first.values(), copy_of_first.values()), summ);	combine_values<double, double> combine_vals = combine_values<double, double>(zip_values<double, double>(copy_of_first.values(), copy_of_first.values()), summ);
	// combine_values<double, double> combine_vals = combine_values<double, double>(copy_of_first.values(), copy_of_first.values(), summ);	
	combine_values<double, double> combine_vals = combine_values<double, double>(100000.0, copy_of_first.values(), summ);
	//combine_values<double, double> combine_vals = combine_values<double, double>(zip_values<double, double>(copy_of_first.values(), copy_of_first.values()), summ);

	template_vector_<double> combines = template_vector_<double>(combine_vals);

	std::cout << "first                  :" << first         << "\n";
	std::cout << "copy_first             :" << copy_first    << "\n";
	std::cout << "copy_of_first          :" << copy_of_first << "\n";
	std::cout << "combines               :" << combines << "\n";
	std::cout << "combines               :" << template_vector_<double>(combines.combine<double>(combines, diff)) << "\n";
	std::cout << "first.insert(0, 13)    :" << first.insert(0, 13)  << "\n";
	std::cout << "first.insert(2, 13)    :" << first.insert(2, 13)  << "\n";
	std::cout << "first.insert(13,13)    :" << first.insert(13, 13) << "\n";
									     
	std::cout << "first.remove_at(0)     :" << first.remove_at(0) << "\n";
	std::cout << "first.remove_at(2)     :" << first.remove_at(2) << "\n";
	std::cout << "first.remove_at(1)     :" << first.remove_at(1) << "\n";
	std::cout << "copy_first             :" << copy_first     << "\n";
	auto val = copy_first[slice(3, 6)];
	std::cout << "copy_first[slice(3, 6)]:" << val << "\n";
 	// copy_first[slice(3, 6)] = template_vector_<double>({ 0, 0, 0 });
  	auto v = copy_first[slice(3, 6)];
	v.apply([](const double v) {return -1.0; });
	std::cout << "copy_first[slice(3, 6)]:" << copy_first[slice(3, 6)] << "\n";
	std::cout << "copy_first             :" << copy_first << "\n";
}

