#pragma once

#include <algorithm>
#include <vcruntime.h>
#include <vector>
template <typename T>
class Range {
public:
    Range(size_t first = 0, const std::vector<T>& data = {});
    bool contains(size_t index) const;
    bool intersect(const Range<T>& other) const;
    void merge(const Range<T>& other);

    size_t getFirst() const;
    size_t getLast() const;

    void setFirst(size_t first);
    void setData(const std::vector<T>& data);

    std::vector<T> getData() const;

private:
    size_t first = 0;
    std::vector<T> data;
};

template <typename T>
class RangeArray {
public:
    void add(Range<T> other);
    size_t getSize() const;
    bool isEmpty() const;
    void clear();

private:
    std::vector<Range<T>> ranges;

public:
    typename decltype(ranges)::const_iterator begin() {
        return std::begin(ranges);
    }
    typename decltype(ranges)::const_iterator end() {
        return std::end(ranges);
    }
};

template <typename T>
Range<T>::Range(size_t fiest, const std::vector<T>& data)
    : first(fiest), data(data) {}

template <typename T>
bool Range<T>::contains(size_t index) const {
    return (index >= first && index < first + data.size());
}

template <typename T>
bool Range<T>::intersect(const Range<T>& other) const {
    return contains(other.getFirst()) || contains(other.getLast()) ||
           other.contains(getFirst()) || other.contains(getLast());
}

template <typename T>
void Range<T>::merge(const Range<T>& other) {
    if (intersect(other)) {
        if (getFirst() < other.getFirst() && getLast() <= other.getLast()) {
            data.resize(data.size() + (other.getLast() - getLast()));
            std::copy(
                std::begin(other.data), std::end(other.data),
                std::next(std::begin(data), other.getFirst() - getFirst()));
        } else if (getFirst() >= other.getFirst() &&
                   getLast() > other.getLast()) {
            decltype(data) data_temp;
            data_temp = other.data;
            data_temp.resize(data_temp.size() + (getLast() - other.getLast()));
            std::copy(
                std::next(std::begin(data), other.getLast() - getFirst()),
                std::end(data),
                std::next(std::begin(data_temp), other.data.size()));
            first = other.first;
            data = data_temp;
        } else if (getFirst() < other.getFirst() &&
                   getLast() > other.getLast()) {
            std::copy(
                std::begin(other.data), std::end(other.data),
                std::next(std::begin(data), other.getFirst() - getFirst()));
        } else if (getFirst() >= other.getFirst() &&
                   getLast() <= other.getLast()) {
            first = other.getFirst();
            data = other.data;
        } else {
            assert(false);
        }
    }
}

template <typename T>
size_t Range<T>::getFirst() const {
    return first;
}

template <typename T>
size_t Range<T>::getLast() const {
    return first + data.size();
}

template <typename T>
std::vector<T> Range<T>::getData() const {
    return data;
}

template <typename T>
void Range<T>::setFirst(size_t first_) {
    first = first_;
}

template <typename T>
void Range<T>::setData(const std::vector<T>& data_) {
    data = data_;
}

template <typename T>
void RangeArray<T>::add(Range<T> other) {
    bool merged = false;
    size_t i = 0;
    for (; i < ranges.size(); ++i) {
        if (other.getLast() < ranges[i].getFirst()) {
            ranges.insert(std::next(std::begin(ranges), i), other);
            merged = true;
            break;
        }
        if (ranges[i].intersect(other)) {
            ranges[i].merge(other);
            merged = true;
            break;
        }
    }

    if (!merged) {
        ranges.push_back(other);
    } else {
        while (i + 1 < ranges.size() &&
               ranges[i].getLast() >= ranges[i + 1].getFirst()) {
            ranges[i + 1].merge(ranges[i]);
            ranges.erase(std::next(std::begin(ranges), i));
        }
    }
}

template <typename T>
size_t RangeArray<T>::getSize() const {
    return ranges.size();
}

template <typename T>
void RangeArray<T>::clear() {
    ranges.clear();
}

template <typename T>
bool RangeArray<T>::isEmpty() const {
    return ranges.empty();
}
