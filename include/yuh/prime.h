#pragma once

#include <cmath>

#include <boost/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>

namespace yuh
{
	typedef long long prime_type;
	namespace range_detail	
	{
		/**
		 * 素数を走査するイテレータ
		 */
		class prime_iterator
			: public boost::iterator_facade<
			prime_iterator,
			prime_type,
			boost::random_access_traversal_tag,
			prime_type,
			std::ptrdiff_t
			>
		{
			/**
			 * 基底クラス(iterator_facade)
			 */
			typedef boost::iterator_facade<
				prime_iterator,
				prime_type,
				boost::random_access_traversal_tag,
				prime_type,
				std::ptrdiff_t
				> base_t;

			/**
			 * 素数リスト
			 */
			static std::vector<value_type> p_;
			
		public:
			/**
			 * 渡された値型
			 */
			typedef typename base_t::value_type value_type;
			/**
			 * イテレータの距離を示す型
			 */
			typedef typename base_t::difference_type difference_type;
			/**
			 * 参照を剥がした際の型
			 */
			typedef typename base_t::reference reference;

			/**
			 * コンストラクタ
			 * @param index 初期値(index番目の素数を表す)
			 */
			prime_iterator(int index = 1);

			/**
			 * 素数リストのサイズを少なくともindex以上になるよう拡大
			 * @param index このindexが確実にリストに存在するようにする
			 */
			static void expand(int index);

			/**
			 * ふるいにかけて素数リストを生成
			 * @param upper ふるいにかける値の上限
			 */
			static void sift(value_type upper);
			
			/**
			 * 素数リストを取得する
			 * @return 素数リスト(vector)
			 */
			static const decltype(p_)& get_prime();

			/**
			 * n番目の素数の概算値を返す 実際より大きいか小さいかは不明
			 */
			static value_type prime_approx(size_t n);
		private:
			/**
			 * operator++で呼ばれる
			 */
            void increment();
			
            /**
			 * operator--で呼ばれる
			 */
            void decrement();
			/**
			 * 等値チェック
			 *
			 * @param rhs operator==の右辺に取る値
			 * @return 内部表現が等しければ真 
			 */
            bool equal(const prime_iterator& rhs) const;
			/**
			 * 参照剥がし
			 * 
			 * @return このイテレータが指すフィボナッチ数
			 */
            reference dereference() const;
			/**
			 * for iterator_facade
			 */
            void advance(difference_type offset);

            difference_type distance_to(const prime_iterator& rhs) const;

			friend class ::boost::iterator_core_access;
			
			/**
			 * 内部表現・index
			 */
			int index_;
		};
	} // namespace range_detail

	/**
	 * 素数列の範囲
	 * 無限数列・有限数列どちらも表現可能
	 */
	class prime_range
	  : public boost::iterator_range< range_detail::prime_iterator >
	  {
	    /**
	     * イテレータの型
	     */
	    typedef range_detail::prime_iterator iterator_t;
	    /**
	     * 基底クラス(iterator_range)
	     */
	    typedef boost::iterator_range<iterator_t> base_t;
	  public:
	    /**
	     * 有限素数列 index指定 [first, last)
	     * @param first 範囲の始め(含む)
	     * @param last  範囲の終わり(含まない)
	     * @return n in [first, last)の素数列a_n
	     */
	  prime_range(int first, int last)
            : base_t(iterator_t(first), iterator_t(last))
	      { }
	    /**
	     * 無限素数列 index指定 [first, Inf)
	     * 単純にalgorithmに渡したりすると無限ループなので注意
	     * @param first 範囲の始め(含む)
	     * @return n in [first, Inf)の素数列a_n
	     */
	  prime_range(int first)
            : base_t(iterator_t(first), iterator_t(0))
	      { }
		
	  };

	/**
	 * 無限素数列 index指定 [first, Inf)
	 * 単純にalgorithmに渡したりすると無限ループなので注意
	 *
	 * @param first 範囲の始め(含む)
	 * @return n in [first, Inf)の素数列を表すrange
	 */
	prime_range prime(int first = 1);
	
	/**
	 * 有限素数列 index指定 [first, last)
	 *
	 * @param first 範囲の始め(含む)
	 * @param last  範囲の終わり(含まない)
	 * @return n in [first, last)の素数列を表すrange
	 */
	prime_range prime(int first, int last);

	/**
	 * エラトステネスの篩にかけてまとめて素数リストに追加する
	 * @param upper この数値未満の数を対象とする
	 */
	inline void prime_sift(prime_type upper)
	{
		//ただのラッパ 階層が深いのでこれだけ表に出しておく
		range_detail::prime_iterator::sift(upper);
	}
	

	/**
	 * 素因数分解
	 * @param Integer 整数型
	 * @param num 対象とする数
	 * @return 素因数リスト(vector)
	 */
	template <typename Integer>
	std::vector<prime_type> prime_factor(Integer num)
	{
		std::vector<prime_type> ret;
		auto n = num;
	
		//ただ順番に割っていくだけ
		for ( auto p: prime() )
		{
			if(p*p > n) break;

			while(n % p == 0)
			{
				n /= p;
				ret. push_back(p);
			}
		}
		//最後に残っているのも素数
		if(n > 0) ret.push_back(n);

		return ret;
	}
}

