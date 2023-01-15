#pragma once

#include <cstddef>

namespace ion::ct
{
	template<typename T = void, typename CounterType = std::size_t>
	struct Counter
	{
	public:
		using CountType = CounterType;

	private:
		template<auto Id>
		struct counter
		{
			using tag = counter;

			struct generator
			{
				friend consteval auto is_defined(tag) { return true; }
			};

			friend consteval auto is_defined(tag);

			template<typename Tag = tag, auto = is_defined(Tag{}) >
			static consteval auto exists(auto) { return true; }

			static consteval auto exists(...) { return generator(), false; }
		};

	public:
		template<auto Id = CounterType{}, auto = [] { } >
		[[nodiscard]] static consteval auto next()
		{
			if constexpr (counter<Id>::exists(Id))
			{
				return next<Id + 1>();
			}
			else
			{
				return Id;
			}
		}
	};

}