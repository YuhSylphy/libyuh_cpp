#pragma once
#define BOOST_RESULT_OF_USE_DECLTYPE

#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>

#include <vector>
#include <functional>
namespace yuh
{
	/**
	 * 自己組織化マップ
	 * @param Input 入力ベクトル型
	 * @param Ref 参照ベクトル型
	 * @param Dist 入力ベクトルの距離関数
	 */
	template<
		typename Input,
		typename Ref
	>
	class som
	{
	public:
		typedef std::function<double(Input const&, Ref const&)> distance;
		typedef std::function<
			std::vector<std::tuple<std::vector<int>, double>>
			(int, std::vector<int> const&) > neighbor;
		typedef std::function<Ref(Ref const&, float, int, Input const&)> update;

	private:

#pragma region field
		/**
		 * 経過学習回数
		 */
		int t_;

		/**
		 * 参照ベクトルリスト
		 */
		std::vector<Ref> refs_;

		/**
		 * 競合層の次元数
		 */
		std::vector<int> ref_dim_;

		/**
		 * 距離関数
		 */
		distance d_;

		/**
		 * 近傍関数
		 */
		neighbor th_;
		
		/**
		 * 更新式
		 */
		update update_;

#pragma endregion
	public:
#pragma region accessor
		/**
		 * 経過学習回数
		 */
		int T() const{ return t_; };
		/**
		 * 参照ベクトルリスト
		 */
		std::vector<Ref> const& refs(){ return refs_; }
#pragma endregion

#pragma region ctor
		template<
			typename Range,
			typename Factory
		>
		som(
			Range const& size,
			Factory factory,
			distance dist,
			neighbor th,
			update update
			) 
			: t_(0)
			, refs_()
			, ref_dim_()
			, d_(dist)
			, th_(th)
			, update_(update)
		{
			// 次元数
			ref_dim_.reserve(boost::distance(size));
			boost::copy(size, std::back_inserter(ref_dim_));

			// 参照ベクトル初期化
			auto l = boost::accumulate(ref_dim_, 1, std::multiplies<int>());
			refs_.reserve(l);
			std::generate_n(std::back_inserter(refs_), l, factory);
		}
#pragma endregion

#pragma region static
		template<typename Range>
		static int rad_conv(Range const& rng, int base)
		{
			BOOST_CONCEPT_ASSERT(( boost::ForwardRangeConcept<Range> ));
			typedef boost::range_value<Range>::type value;
			return boost::accumulate(
				rng, value(), 
				[base](value ret, value x){ return ret * base + x; }
			);
		}

		static std::vector<int> rad_conv(int n, int base)
		{

			return std::vector<int>();
			
		}

#pragma endregion
	};
}