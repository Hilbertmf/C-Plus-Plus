#include <algorithm>  /// For std::min and std::max
#include <cassert>    /// For assert
#include <iostream>   /// For IO operations
#include <vector>     /// For std::vector

/*
 * @namespace
 * @brief Data structures
 */
namespace data_structures {
/**
 * @brief class representation of the segment tree with lazy propagation
 * @tparam T The type of the class that goes in the datastructure
 */
template <class T>
class SegmentTree {
 private:
    const T ID = 0;       ///< Comb(ID, x) = x
    std::vector<T> t;     ///< Vector to represent the tree
    std::vector<T> lazy;  ///< Lazy propagation array
    int size = 0;  ///< Number of elements available for querying in the tree

 private:
    /**
     * @brief Any associative function that combines x and y
     * @param x The first operand
     * @param y The second operand
     * @return Some associative operation applied to these two values. In this
     * case, I used addition
     */
    T comb(T x, T y) { return x + y; }

    /**
     * @brief Gives the midpoint between two integers
     * @param l The left endpoint
     * @param r The right endpoint
     * @return the middle point between them
     */
    int mid(int l, int r) { return l + (r - l) / 2; }

    /**
     * @brief Propagates the lazy value down to child nodes
     * @param i The index of the current node
     * @param l The leftmost node of the current node
     * @param r The rightmost node of the current node
     */
    void propagate(int i, int l, int r) {
        if (lazy[i] != 0) {
            t[i] += lazy[i] * (r - l + 1);
            if (l != r) {
                lazy[i * 2] += lazy[i];
                lazy[i * 2 + 1] += lazy[i];
            }
            lazy[i] = 0;
        }
    }

    /**
     * @brief Helper method for point updates
     * @param i The index of the current node
     * @param l The leftmost node of the current node
     * @param r The rightmost node of the current node
     * @param pos The position to update
     * @param val The value to update it to
     */
    void update(int i, int l, int r, int pos, T val) {
        propagate(i, l, r);  // Ensure the node is up to date
        if (l == r) {
            t[i] = val;
        } else {
            int m = mid(l, r);
            if (pos <= m) {
                update(i * 2, l, m, pos, val);
            } else {
                update(i * 2 + 1, m + 1, r, pos, val);
            }
            t[i] = comb(t[i * 2], t[i * 2 + 1]);
        }
    }

    /**
     * @brief Helper method for range updates with lazy propagation
     * @param i The current node
     * @param l The leftmost node of the current node
     * @param r The rightmost node of the current node
     * @param tl The left endpoint of the range
     * @param tr The right endpoint of the range
     * @param val The value to add to the range
     */
    void updateRange(int i, int l, int r, int tl, int tr, T val) {
        propagate(i, l, r);  // Ensure the node is up to date
        if (tl > r || tr < l)
            return;  // Out of range
        if (tl <= l && r <= tr) {
            lazy[i] += val;  // Apply lazy update
            propagate(i, l,
                      r);  // Propagate immediately to update the current node
        } else {
            int m = mid(l, r);
            updateRange(i * 2, l, m, tl, tr, val);
            updateRange(i * 2 + 1, m + 1, r, tl, tr, val);
            t[i] = comb(t[i * 2], t[i * 2 + 1]);
        }
    }

    /**
     * @brief Helper method for range_comb method below
     * @param i The current node
     * @param l The leftmost node of the current node
     * @param r The rightmost node of the current node
     * @param tl The left endpoint of the range
     * @param tr The right endpoint of the range
     * @return The comb operation applied to all values between tl and tr
     */
    T range_comb(int i, int l, int r, int tl, int tr) {
        propagate(i, l, r);  // Ensure the node is up to date
        if (tl > r || tr < l)
            return ID;  // Out of range
        if (tl <= l && r <= tr)
            return t[i];  // Fully in range
        int m = mid(l, r);
        return comb(range_comb(i * 2, l, m, tl, std::min(tr, m)),
                    range_comb(i * 2 + 1, m + 1, r, std::max(tl, m + 1), tr));
    }

 public:
    /**
     * @brief Constructor to initialize the segment tree
     * @param n The number of elements in the array
     */
    explicit SegmentTree(int n) : t(n * 4, ID), lazy(n * 4, 0), size(n) {}

    /**
     * @brief Updates a value at a certain position
     * @param pos The position to update
     * @param val The value to update it to
     */
    void update(int pos, T val) { update(1, 1, size, pos, val); }

    /**
     * @brief Updates a range of values
     * @param l The left endpoint of the range
     * @param r The right endpoint of the range
     * @param val The value to add to each element in the range
     */
    void updateRange(int l, int r, T val) {
        updateRange(1, 1, size, l, r, val);
    }

    /**
     * @brief Returns comb across all values between l and r
     * @param l The left endpoint of the range
     * @param r The right endpoint of the range
     * @return The value of the comb operations
     */
    T range_comb(int l, int r) { return range_comb(1, 1, size, l, r); }
};
}  // namespace data_structures

/**
 * @brief Self-test implementations
 * @returns void
 */
static void test() {
    data_structures::SegmentTree<int> t(5);
    t.update(1, 1);
    t.update(2, 2);
    t.update(3, 3);
    t.update(4, 4);
    t.update(5, 5);
    assert(t.range_comb(1, 3) == 6);  // 1 + 2 + 3 = 6
    t.update(1, 3);
    assert(t.range_comb(1, 3) == 8);  // 3 + 2 + 3 = 8

    t.updateRange(2, 4, 2);  // Increment values from index 2 to 4 by 2
    assert(t.range_comb(2, 4) == 15);  // 4 + 5 + 6 = 15

    std::cout << "All tests have successfully passed!\n";
}

/**
 * @brief Main function
 * @returns 0 on exit
 */
int main() {
    test();  // run self-test implementations
    return 0;
}
