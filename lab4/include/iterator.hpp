#include <boost/iterator/iterator_adaptor.hpp>

template <typename Iterator>
class image_iterator 
    : public boost::iterator_adaptor<image_iterator<Iterator>, Iterator> 
{
private:
    friend class boost::iterator_core_access;

    size_t m_width; 
    size_t m_stride; 
    size_t m_col; 

    void increment() 
    {
        if (m_col + 1 < m_width) 
        {
            ++this->base_reference(); 
            ++m_col;
        }
        else 
        {
            this->base_reference() = this->base() - m_col + m_stride; 
            m_col = 0;
        }
    }

    void decrement() 
    {
        if (m_col > 0)
        {
            --this->base_reference();
            --m_col;
        }
        else
        {
            auto prev_row_start = this->base() - m_stride;
            this->base_reference() = prev_row_start + (m_width - 1);
            m_col = m_width - 1;
        }
    }

    void advance(typename image_iterator::difference_type n) 
    {
        if (n == 0) return;

        auto ww = static_cast<long long>(m_width);
        auto total = static_cast<long long>(m_col) + n;
        auto mod = ((total % ww) + ww) % ww;
        auto row_offset = (total - mod) / ww;

        auto current_row_start = this->base() - m_col;
        auto new_row_start = current_row_start + row_offset * static_cast<typename image_iterator::difference_type>(m_stride);

        this->base_reference() = new_row_start + static_cast<typename image_iterator::difference_type>(mod);
        m_col = static_cast<size_t>(mod);
    }

    typename image_iterator::difference_type distance_to(const image_iterator& other) const 

    {
        auto current_row_start = this->base() - m_col;
        auto other_row_start = other.base() - other.m_col;
        auto row_diff = (other_row_start - current_row_start) / static_cast<typename image_iterator::difference_type>(m_stride); 
        auto col_diff = static_cast<typename image_iterator::difference_type>(other.m_col - m_col);
        return row_diff * static_cast<typename image_iterator::difference_type>(m_width) + col_diff; 
    }

public:
    image_iterator() : m_width(0), m_stride(0), m_col(0) {}

    image_iterator(Iterator it, size_t width, size_t stride)
        : image_iterator::iterator_adaptor_(it), m_width(width), m_stride(stride), m_col(0)
    {} 
};