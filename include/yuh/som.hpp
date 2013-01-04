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
	 * ���ȑg�D���}�b�v
	 * @param Input ���̓x�N�g���^
	 * @param Ref �Q�ƃx�N�g���^
	 * @param Dist ���̓x�N�g���̋����֐�
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
		 * �o�ߊw�K��
		 */
		int t_;

		/**
		 * �Q�ƃx�N�g�����X�g
		 */
		std::vector<Ref> refs_;

		/**
		 * �����w�̎�����
		 */
		std::vector<int> ref_dim_;

		/**
		 * �����֐�
		 */
		distance d_;

		/**
		 * �ߖT�֐�
		 */
		neighbor th_;
		
		/**
		 * �X�V��
		 */
		update update_;

#pragma endregion
	public:
#pragma region accessor
		/**
		 * �o�ߊw�K��
		 */
		int T() const{ return t_; };
		/**
		 * �Q�ƃx�N�g�����X�g
		 */
		std::vector<Ref> const& refs(){ return refs_; }
#pragma endregion

#pragma region ctor
		/**
		 * som
		 * @param size �e�����̃T�C�Y
		 * @param factory �Q�ƃx�N�g���������֐�
		 * @param dist �����֐�d ���͂ƎQ�Ƃ̋���
		 * @param th �ߖT�֐��� �ߖT�̌`
		 * @param update ���҃m�[�h�X�V�֐� 
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
			// ������
			ref_dim_.reserve(boost::distance(size));
			boost::copy(size, std::back_inserter(ref_dim_));

			// �Q�ƃx�N�g��������
			auto l = boost::accumulate(ref_dim_, 1, std::multiplies<int>());
			refs_.reserve(l);
			std::generate_n(std::back_inserter(refs_), l, factory);
		}
#pragma endregion

#pragma region static
		/**
		 * ������->�������k
		 * @param rng ������
		 * @param base �
		 * @return ���k�㐮��
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
		 * ����->�������
		 * @param n ���̐�
		 * @param base �ϊ���
		 * @param len �z��
		 * @return ��ɉ������T�C�Ylen��vector
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
		{   // ���w�����_+concept�Ƃ��~����
			template<typename T>
			double operator()(double sum, T const& t)
			{
				return sum + std::pow(boost::get<0>(t) - boost::get<1>(t), 2);
			}
		};
	public:
		/**
		 * ���[�N���b�h����
		 * @param input ���̓x�N�g��
		 * @param ref �Q�ƃx�N�g��
		 * @return ���[�N���b�h����
		 */
		static double euc_dist(Input const& input, Ref const& ref) 
		{
			return std::sqrt(boost::accumulate(yuh::combine(input, ref), .0, euc_func()));
		}
		/**
		 * �ߖT�֐�/��`�Œ� �񎟌�
		 * @param winner ���҃m�[�h�̃C���f�b�N�X
		 * @param T �^�[����(���g�p
		 * @return ��`�͈�
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

			// ����
			std::vector<int> pos = rad_extract(winner, dim);

			std::vector<std::tuple<int, double>> ret =
				  boost::irange(-1, 2) 
				| iorate::product(boost::irange(-1, 2))									// (x, y)�̗�
				| transformed([&](boost::tuple<int, int> t)
				{ 
					return vec(boost::get<0>(t),  boost::get<1>(t)); 	
				} )					// vector�ɕϊ�
				| transformed([=](std::vector<int> v)
				{
					double rate = .0;
					switch (boost::accumulate(v | transformed([](int i){return i*i;}), 0))
					{
					case 0: rate = just;    break;
					case 1: rate = touched; break;
					case 2: rate = angle;   break;
					default: throw std::exception("rect_neighbor ��`�͈͊O");
					}
					return std::make_tuple(v, rate);
				} )						// rate��t�^
				| transformed([&](std::tuple<std::vector<int>, double> t) 
				{
					auto v = std::get<0>(t);
					boost::for_each(boost::combine(v, pos),
						[](boost::tuple<int&, int> s) {
							boost::get<0>(s) += boost::get<1>(s);
						});
					return std::make_tuple(v, std::get<1>(t));
				} )	// ���҃m�[�h���S�Ɉړ�
				| filtered([&](std::tuple<std::vector<int>, double> t)
				{
					auto& v = std::get<0>(t);
					return boost::algorithm::all_of(
						boost::combine(v, dim),
						[](boost::tuple<int, int> s) {
							return 0 <= boost::get<0>(s) && boost::get<0>(s) < boost::get<1>(s);
						});
				} )		// �͈͊O�J�b�g
				| transformed([&](std::tuple<std::vector<int>, double> t) 
				{
					return std::make_tuple(rad_compress(std::get<0>(t), dim), std::get<1>(t));
				} )	// ���W�\�����k
				| boost::as_container;

			return ret;
		}

		/**
		 * �ߖT�֐�/��`�Œ� �񎟌�
		 * @param winner ���҃m�[�h�̃C���f�b�N�X
		 * @param T �^�[����(���g�p
		 * @return ��`�͈�
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
		 * 1�X�e�b�v
		 * @param input ���̓x�N�g��
		 * @return ���҃m�[�h�ւ�iterator
		 */
		typename std::vector<Ref>::iterator step(Input const& input)
		{
			using std::placeholders::_1;
			auto m = boost::min_element(refs_, 
				[&](Ref& x, Ref& y)->bool{return d_(input, x) < d_(input, y);}
			);
			auto index = std::distance(std::begin(refs_), m);
			auto pos = rad_extract(index);

			// �ߖT
			auto neighbor = th_(index, t_, ref_dim_);

			for ( auto& t: neighbor )
			{
				auto index = std::get<0>(t); //�ߖT��index
				auto rate = std::get<1>(t);//�e���䗦

				refs_[index] = update_(refs_[index], rate, t_, input);
			}

			t_++;

			return m;
		}

#pragma endregion
	};
}