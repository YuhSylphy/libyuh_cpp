#pragma once
#define BOOST_RESULT_OF_USE_DECLTYPE


#include <product.hpp>

#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/adaptor/memoized.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/regular_extension/transformed.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/regular_extension/filtered.hpp>
#include <boost/range/as_container.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>


#include <vector>
#include <functional>


namespace yuh
{
	using namespace boost::adaptors;
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
		/** T => (index, rate) */
		typedef std::function<std::vector<std::tuple<int, double>>(int, int, std::vector<int>const&)> neighbor;
		typedef std::function<Ref(Ref const&, double, int, Input const&)> update;

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
		/**
		 * som
		 * @param size 各次元のサイズ
		 * @param factory 参照ベクトル初期化関数
		 * @param dist 距離関数d 入力と参照の距離
		 * @param th 近傍関数Θ 近傍の形
		 * @param update 勝者ノード更新関数 
		 */
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
		/**
		 * 整数列->整数圧縮
		 * @param rng 整数列
		 * @param base 基数
		 * @return 圧縮後整数
		 */
		template<typename Range, typename RangeB>
		static int rad_compress(Range const& rng, RangeB const& base)
		{
			BOOST_CONCEPT_ASSERT(( boost::ForwardRangeConcept<Range> ));
			BOOST_CONCEPT_ASSERT(( boost::ForwardRangeConcept<RangeB> ));
			typedef boost::range_value<Range>::type value;
			typedef boost::range_value<RangeB>::type base_type;
			return boost::accumulate(
				boost::combine(base, rng) | reversed, 0, 
				[&](int ret, boost::tuple<base_type, value> x){
					return ret * boost::get<0>(x) + boost::get<1>(x);
				}
			);
		}

		/**
		 * 整数->整数列解凍
		 * @param n 元の数
		 * @param base 変換後基数
		 * @param len 配列長
		 * @return 基数に応じたサイズlenのvector
		 */
		template<typename T, typename Range>
		static std::vector<T> rad_extract(T n, Range const& base)
		{
			BOOST_CONCEPT_ASSERT(( std::is_integral<T> ));
			std::vector<T> ret;
			ret.reserve(boost::distance(base));
			for ( auto b: base )
			{
				ret.push_back( n % b );
				n /= b;
			}

			return ret;
		}

	private:
		struct euc_func
		{   // 多層ラムダ+conceptとか欲しい
			template<typename T>
			double operator()(double sum, T const& t)
			{
				return sum + std::pow(boost::get<0>(t) - boost::get<1>(t), 2);
			}
		};
	public:
		/**
		 * ユークリッド距離
		 * @param input 入力ベクトル
		 * @param ref 参照ベクトル
		 * @return ユークリッド距離
		 */
		static double euc_dist(Input const& input, Ref const& ref) 
		{
			return std::sqrt(boost::accumulate(yuh::combine(input, ref), .0, euc_func()));
		}
		/**
		 * 近傍関数/矩形固定 二次元
		 * @param winner 勝者ノードのインデックス
		 * @param T ターン数(未使用
		 * @return 矩形範囲
		 */
		static std::vector<std::tuple<int, double>> rect_neighbor(int winner, int T, std::vector<int> const& dim)
		{ 
			double const angle = .05;
			double const touched = .1;
			double const just = .4;

			auto const vec = [](int x, int y){
				std::vector<int> ret = oven::initial_values(x, y);
				return ret;
			};

			// 勝者
			std::vector<int> pos = rad_extract(winner, dim);

			std::vector<std::tuple<int, double>> ret =
				  boost::irange(-1, 2) 
				| iorate::product(boost::irange(-1, 2))									// (x, y)の列
				| transformed([&](boost::tuple<int, int> t)
				{ 
					return vec(boost::get<0>(t),  boost::get<1>(t)); 	
				} )					// vectorに変換
				| transformed([=](std::vector<int> v)
				{
					double rate = .0;
					switch (boost::accumulate(v | transformed([](int i){return i*i;}), 0))
					{
					case 0: rate = just;    break;
					case 1: rate = touched; break;
					case 2: rate = angle;   break;
					default: throw std::exception("rect_neighbor 矩形範囲外");
					}
					return std::make_tuple(v, rate);
				} )						// rateを付与
				| transformed([&](std::tuple<std::vector<int>, double> t) 
				{
					auto v = std::get<0>(t);
					boost::for_each(boost::combine(v, pos),
						[](boost::tuple<int&, int> s) {
							boost::get<0>(s) += boost::get<1>(s);
						});
					return std::make_tuple(v, std::get<1>(t));
				} )	// 勝者ノード中心に移動
				| filtered([&](std::tuple<std::vector<int>, double> t)
				{
					auto& v = std::get<0>(t);
					return boost::algorithm::all_of(
						boost::combine(v, dim),
						[](boost::tuple<int, int> s) {
							return 0 <= boost::get<0>(s) && boost::get<0>(s) < boost::get<1>(s);
						});
				} )		// 範囲外カット
				| transformed([&](std::tuple<std::vector<int>, double> t) 
				{
					return std::make_tuple(rad_compress(std::get<0>(t), dim), std::get<1>(t));
				} )	// 座標表現圧縮
				| boost::as_container;

			return ret;
		}

		/**
		 * 近傍関数/矩形固定 二次元
		 * @param winner 勝者ノードのインデックス
		 * @param T ターン数(未使用
		 * @return 矩形範囲
		 */
		template<size_t c_n, size_t c_d, size_t T>
		static std::vector<std::tuple<int, double>> quad_neighbor(int winner, int t, std::vector<int> const& dim)
		{
			static_assert( c_d > 0, "c_d requires to be > 0: quad_neighbor");
			static const double c = static_cast<double>(c_n) / c_d;
			auto dis_2 = [](std::vector<int> const&w, std::vector<int> const&m)
			{
				return boost::accumulate(
						  boost::combine(w, m) 
						| transformed([](boost::tuple<int, int> const& t) { return std::pow(boost::get<0>(t) - boost::get<1>(t), 2); }), 
						.0);
			};
			double const alpha = t >= T ? 1 - static_cast<double>(T-1) / T : 1 - static_cast<double>(t) / T;
			auto win = rad_extract(winner, dim);
			std::vector<std::tuple<int, double>> ret = 
				  boost::irange(0, boost::accumulate(dim, 1, std::multiplies<int>()))
				| transformed([&](int index)
				{
					auto v = som::rad_extract(index, dim);
					return std::make_tuple(index, 
						c * std::exp(- dis_2(win, v) / (alpha * alpha) ) );
				} )
				| boost::as_container
				;
			return ret;
		}

#pragma endregion

#pragma region method
		template<typename Range>
		inline int rad_compress(Range const& rng)
		{
			return rad_compress(rng, ref_dim_);
		}

		template<typename T>
		inline std::vector<T> rad_extract(T n)
		{
			return rad_extract(n, ref_dim_);
		}

		/**
		 * 1ステップ
		 * @param input 入力ベクトル
		 * @return 勝者ノードへのiterator
		 */
		typename std::vector<Ref>::iterator step(Input const& input)
		{
			using std::placeholders::_1;
			auto m = boost::min_element(refs_, 
				[&](Ref& x, Ref& y)->bool{return d_(input, x) < d_(input, y);}
			);
			auto index = std::distance(std::begin(refs_), m);
			auto pos = rad_extract(index);

			// 近傍
			auto neighbor = th_(index, t_, ref_dim_);

			for ( auto& t: neighbor )
			{
				auto index = std::get<0>(t); //近傍のindex
				auto rate = std::get<1>(t);//影響比率

				refs_[index] = update_(refs_[index], rate, t_, input);
			}

			t_++;

			return m;
		}

#pragma endregion
	};
}