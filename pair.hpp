// https://www.slamecka.cz/posts/2021-03-17-cpp-metaprogramming-exercises-1/
//
// This is a set of exercises for type level programming in C++, suitable for
// beginners or as a refresher for intermediate programmers.
//
// Scope:
// - Most exercises in this set are confined to vectors of integers for simplicity.
// - Exercises are centered around basic functional techniques since they are easy to express with template metaprogramming.
//
// How to solve it:
// - You should be able to compile with gcc version 10.2, g++ --std=c++20 typelevel_set1.cpp
// - You should understand the requested behaviours from static_assert-s,
//   except for print, which comes with a test in main().
// - Tests are commented-out and you should uncomment them as you go.
// - You might want to read some introductory material before attempting these:
//
//   https://en.cppreference.com/w/cpp/language/templates
//
//   https://en.cppreference.com/w/cpp/language/parameter_pack
//
//   https://en.cppreference.com/w/cpp/language/template_parameters
//      in this set we'll only see non-type template parameters and type template parameters
//
//   https://en.cppreference.com/w/cpp/header/type_traits
//
//   https://en.cppreference.com/w/cpp/language/partial_specialization
//      also see https://stackoverflow.com/questions/17005985/what-is-the-partial-ordering-procedure-in-template-deduction
//

#include <iostream>
#include <type_traits>

namespace {

	/**
	 * 1. Define Vector, a template level list of integers.
	 * Hint: Use non-type template parameter pack.
	 */

	 // Your code goes here:
	template <int ... types>
	struct Vector {};
	 // ^ Your code goes here

	  static_assert(std::is_same_v<Vector<1,2>, Vector<1,2>>);
	  static_assert(std::is_same_v<Vector<2,3,4,5,6>, Vector<2, 3, 4, 5, 6>>);


	 /**
	  * 2. Define function print() that prints Vector-s.
	  * Example: print(Vector<1,2,3>{}); // prints "1 2 3" (newline)
	  * See main() below.
	  */

	  // Your code goes here:
	//template <int ... vals>
	//constexpr void print(Vector<vals...>) {
	//	std::cout << "\n";
	//  }

	  template <int... vals>
	  constexpr void print(Vector<vals...>) {
	  }

	  template <int val>
	  constexpr void print(Vector<val>) {
		  std::cout << val << '\n';;
	  }


	template <int val, int ... vals>
	constexpr void print(Vector<val, vals...>) {
		std::cout << val << ' ';;
		print(Vector<vals...>{});
	}

	// ^ Your code goes here


	  /**
	   * 3. Define Prepend.
	   * Hint: Use `using type = ...` inside a struct that has both non-type and type template parameters.
	   */

	   // Your code goes here
		template <int val, typename T>
		struct Prepend;

		template <int val, int... vals>	
		struct Prepend <val, Vector<vals...>> 
		{
			using type = Vector<val, vals...>;
		};
	   // ^ Your code goes here

	   static_assert(std::is_same_v<Prepend<1, Vector<2,3>>::type, Vector<1,2,3>>);


	   /**
		* 4. Define PrependT that can be used without ::type.
		* Hint: See how enable_if_t is defined in terms of enable_if.
		*
		* This technique is not used further to reduce boilerplate.
		*/

		// Your code goes here:

	   template <int val, typename ... Types>
	   using PrependT = typename Prepend<val, Types ...>::type;
		// ^ Your code goes here

		static_assert(std::is_same_v<PrependT<1, Vector<2,3>>, Vector<1,2,3>>);


		/**
		 * 5. Define Append.
		 */

		 // Your code goes here:
		template <int val, typename T>
		struct Append;

		template <int val, int... vals>
		struct Append <val, Vector<vals...>>
		{
			using type = Vector<vals..., val>;
		};
		// ^ Your code goes here

		static_assert(std::is_same_v< Append<4, Vector<1,2,3>>::type , Vector<1,2,3,4> >);


		 /**
		  * 6. Define PopBack.
		  */

		  // Your code goes here:
		template <typename T>
		struct PopBack;

		template <int val>
		struct PopBack <Vector<val>>
		{
			using type = Vector<>;
		};

		template <int val, int... vals>
		struct PopBack <Vector<val, vals...>>
		{
			using type = PrependT<val, typename PopBack<Vector<vals...>>::type>;
		};

		  // ^ Your code goes here

		 static_assert(std::is_same_v< PopBack<Vector<1,2,3,4>>::type , Vector<1,2,3> >);


		  /**
		   * 7. Define RemoveFirst, that removes the first occurence of element R from vector V.
		   */

		   // Your code goes here:
		// template <int first, typename T>
		// struct RemoveFirst;

		// template <int first, int val, int... vals>
		// struct RemoveFirst<first, Vector<val, vals ...>> 
		// {
		//	 typedef std::conditional<
		//		 (first == val),
		//		 Vector<vals...>,
		//		 RemoveFirst<first, Vector<vals...>::type>
		//	 > type;
		//};

		 template <int first, typename T>
		 struct RemoveFirst;

		 template <int first, int... vals>
		 struct RemoveFirst<first, Vector<first, vals ...>>
		 {
			 using type = Vector<vals...>;
 		 };

		 template <int first, int val, int... vals>
		 struct RemoveFirst<first, Vector<val, vals ...>>
		 {
			 using type = PrependT<val, typename RemoveFirst<first, Vector<vals...>>::type>;
 		 };

		 template <int first, int ... vals>
		 struct RemoveFirst<first, Vector<vals...>>
		 {
			 using type = Vector<>;
 		 };

		   static_assert(std::is_same_v<RemoveFirst<2, Vector<1,2,1,2>>::type, Vector<1,1,2>>);
		   static_assert(std::is_same_v<RemoveFirst<3, Vector<1,2,1,2>>::type, Vector<1,2,1,2>>);


		   /**
			* 8. Define RemoveAll, that removes all occurences of element R from vector V.
			*/

			// Your code goes here:
		   template <int first, typename T>
		   struct RemoveAll;

		   template <int rm, int... vals>
		   struct RemoveAll<rm, Vector<rm, vals ...>>
		   {
			   using type = typename RemoveAll<rm, Vector<vals...>>::type;
		   };

		   template <int rm, int val, int... vals>
		   struct RemoveAll<rm, Vector<val, vals ...>>
		   {
			   using type = PrependT<val, typename RemoveAll<rm, Vector<vals...>>::type>;
		   };

		   template <int first, int ... vals>
		   struct RemoveAll<first, Vector<vals...>>
		   {
			   using type = Vector<>;
		   };
			// ^ Your code goes here

			static_assert(std::is_same_v<RemoveAll<9, Vector<1,9,2,9,3,9>>::type, Vector<1,2,3>>);
			
			/**
			 * 9. Define Length.
			 * Hint: Use `static constexpr int value = ...` inside the struct.
			 */

			 // Your code goes here:
			template <typename T>
			struct Length;

			template <int val, int ... vals>
			struct Length<Vector<val, vals...>>
			{
				constexpr static int value = 1 + Length<Vector<vals...>>::value;
			};

			template <>
			struct Length<Vector<>>
			{
				constexpr static int value = 0;
			};
			 // ^ Your code goes here

			 static_assert(Length<Vector<1,2,3>>::value == 3);


			 /**
			  * 10. Define length, which works like Length<V>::value.
			  * Hint: See how is_same_v is defined in terms of is_same.
			  */

			  // Your code goes here:
			 template <typename T>
			 inline constexpr int length = Length<T>::value;
			 
			 // ^ Your code goes here

			   static_assert(length<Vector<>> == 0);
			   static_assert(length<Vector<1,2,3>> == 3);


			  /**
			   * 11. Define Min, that stores the minimum of a vector in its property `value`.
			   */

			   // Your code goes here:
			   template <int a, int b>
			   struct GetSmaller {
				   static constexpr int smaller = (a <= b) ? a : b;
			   };

			   template <typename T>
			   struct Min {};

			   template <int min, int first, int ... vals>
			   struct Min<Vector<min, first, vals...>>
			   {
				   static constexpr int value = Min<Vector<GetSmaller<min, first>::smaller, vals...>>::value;
			   };

			   template <int min>
			   struct Min<Vector<min>>
			   {
				   static constexpr int value = min;
			   };


			   // ^ Your code goes here

			   static_assert(Min<Vector<1, 2, 3>>::value == 1);
			   static_assert(Min<Vector<100, 22312, 3234, 324234, 4325, 345, 0, 543, 53 >>::value == 0);
			   static_assert(Min<Vector<3,1,2>>::value == 1);
			   static_assert(Min<Vector<3,2,1>>::value == 1);


			   /**
				* 12. Define Sort.
				*/

			   //template <int a, int b>
			   //struct sort_2_vals {
				  // static constexpr bool is_a_smaller = (a <= b);
				  // static constexpr int smaller = is_a_smaller ? a : b;
				  // static constexpr int bigger = is_a_smaller ? b : a;

				  // using type = Vector<smaller, bigger>;
			   //};

			   //template <typename A, typename B>
			   //struct TupleCat;

			   //template <int ... first_vals, int ... second_vals>
			   //struct TupleCat<Vector<first_vals ... >, Vector<second_vals ... >>
			   //{
				  // using type = Vector<first_vals ..., second_vals ...>;
			   //};

				// Your code goes here:
				
				// Implemented with Bubble Sort //
				
			   template <int a, int b>
			   struct is_a_bigger {
				   static constexpr bool value = (a <= b);
			   };

			   template <int a, int b>
			   static constexpr int smaller = is_a_bigger<a,b>::value ? a : b;
			   
			   template <int a, int b>
			   static constexpr int bigger = is_a_bigger<a, b>::value ? b : a;

			   template <typename T>
			   struct Sort_inner;

			   template <int first, int second, int ... vals>
			   struct Sort_inner<Vector<first, second, vals ...>>
			   {
				   using type = PrependT<smaller<first, second>, 
								typename Sort_inner<Vector<bigger<first, second>, vals ...>>::type>;
			   };

			   template <int biggest>
			   struct Sort_inner<Vector<biggest>> {
				   using type = Vector<biggest>;
			   };


			   //template <typename T>
			   //struct SortRecursive;

			   template <typename T>
			   struct SortRecursive {
				   using sort_pass_1_type = typename Sort_inner<T>::type;
				   using sort_pass_2_type = typename Sort_inner<sort_pass_1_type>::type;
				   static constexpr bool is_swapped = !std::is_same_v<sort_pass_1_type, sort_pass_2_type>;
			   };

			   template <bool is_swapped, typename T>
			   struct FinalizeSort;

			   template <typename T>
			   struct FinalizeSort<true, typename T>
			   {
				   //using type = typename FinalizeSort<SortRecursive<T>::is_swapped, SortRecursive<T>::sort_pass_1>::type;
				   using type = typename FinalizeSort<SortRecursive<T>::is_swapped, typename SortRecursive<T>::sort_pass_1_type>::type;
			   };

			   template <typename T>
			   struct FinalizeSort<false, typename T>
			   {
				   using type = T;
			   };

			   template <typename T>
			   struct Sort {
				   using type = typename FinalizeSort<true, T>::type;
			   };

			   // ^ Your code goes here

			    static_assert(std::is_same_v<Sort<Vector<4, 1, 5>>::type, Vector<1, 4, 5>>);
				static_assert(std::is_same_v<Sort<Vector<4,1,2,5,6,3>>::type, Vector<1,2,3,4,5,6>>);
				static_assert(std::is_same_v<Sort<Vector<3,3,1,1,2,2>>::type, Vector<1,1,2,2,3,3>>);
				static_assert(std::is_same_v<Sort<Vector<2,2,1,1,3,3>>::type, Vector<1,1,2,2,3,3>>);


				/**
				 * 13. Define Uniq.
				 */

				 // Your code goes here:
				 // ^ Your code goes here

				 // static_assert(std::is_same_v<Uniq<Vector<1,1,2,2,1,1>>::type, Vector<1,2,1>>);


				 /**
				  * 14. Define type Set.
				  */

				  // Your code goes here:
				  // ^ Your code goes here

				  // static_assert(std::is_same_v<Set<2,1,3,1,2,3>::type, Set<1,2,3>::type>);


				  /**
				   * 15. Define SetFrom.
				   */

				   // Your code goes here:
				   // ^ Your code goes here

				   // static_assert(std::is_same_v<SetFrom<Vector<2,1,3,1,2,3>>::type, Set<1,2,3>::type>);


				   /**
					* 16. Define Get for Vector.
					* Provide an improved error message when accessing outside of Vector bounds.
					*/

					// Your code goes here:
					// ^ Your code goes here

					// static_assert(Get<0, Vector<0,1,2>>::value == 0);
					// static_assert(Get<1, Vector<0,1,2>>::value == 1);
					// static_assert(Get<2, Vector<0,1,2>>::value == 2);
					// static_assert(Get<9, Vector<0,1,2>>::value == 2); // How good is your error message?


					/**
					 * 17. Define BisectLeft for Vector.
					 * Given n and arr, return the first index i such that arr[i] >= n.
					 * If it doesn't exist, return the length of the array.
					 *
					 * Don't worry about complexity but aim for the binary search pattern.
					 *
					 * Hint: You might find it convenient to define a constexpr helper function.
					 */

					 // Your code goes here:
					 // ^ Your code goes here

					 // static_assert(BisectLeft<  3, Vector<0,1,2,3,4>>::value == 3);
					 // static_assert(BisectLeft<  3, Vector<0,1,2,4,5>>::value == 3);
					 // static_assert(BisectLeft<  9, Vector<0,1,2,4,5>>::value == 5);
					 // static_assert(BisectLeft< -1, Vector<0,1,2,4,5>>::value == 0);
					 // static_assert(BisectLeft<  2, Vector<0,2,2,2,2,2>>::value == 1);


					 /**
					  * 18. Define Insert for Vector, it should take position, value and vector.
					  * Don't worry about bounds.
					  * Hint: use the SFINAE Enable trick, e.g.
					  *   template<typename X, typename Enable = void> struct Foo;
					  *   template<typename X> struct<std::enable_if_t<..something      about X..>> Foo {...};
					  *   template<typename X> struct<std::enable_if_t<..something else about X..>> Foo {...};
					  */

					  // Your code goes here:
					  // ^ Your code goes here

					  // static_assert(std::is_same_v<Insert<0, 3, Vector<4,5,6>>::type, Vector<3,4,5,6>>);
					  // static_assert(std::is_same_v<Insert<1, 3, Vector<4,5,6>>::type, Vector<4,3,5,6>>);
					  // static_assert(std::is_same_v<Insert<2, 3, Vector<4,5,6>>::type, Vector<4,5,3,6>>);
					  // static_assert(std::is_same_v<Insert<3, 3, Vector<4,5,6>>::type, Vector<4,5,6,3>>);

}
