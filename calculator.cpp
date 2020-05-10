#include "calculator.hpp"

void Calculator::initialize() {
    m_field.resize(m_height);

    for (auto& row : m_field)
        row.resize(m_width);
}

void Calculator::calculate(const sf::Vector2f center, const float width, const float height) 
{
    auto threads_number = std::thread::hardware_concurrency();

    unsigned int XSize = 1000U, YSize = 1000U;

    auto dx = width / m_width;
    auto dy = height / m_height;

    std::vector < std::thread > threads(threads_number);

    auto size = m_height / threads_number;
    auto tail = m_height - threads_number * size;

    std::complex <float> C{ -0.3, 0.66 };

    auto function = [=](std::size_t id) 
    {
        auto first_y = size * id;
        auto last_y = first_y + size;

        last_y = last_y > m_height ? m_height : last_y;

        auto iteration = 0U;

        for (auto y = first_y; y < last_y; ++y) 
        {
            for (auto x = 0U; x < m_width; ++x) 
            {

                std::complex <float> Z{ 0.0f, 0.0f };

                Z.real(center.x - width / 2.0f + x * dx);
                Z.imag(center.y - height / 2.0f + y * dy);

                for (iteration = 0; (iteration < m_max_iterations) & (abs(Z) < 4.0f); iteration++) {

                    Z = Z * Z + C;

                }

                m_field[y][x] = iteration;
            };
        }
    };

    for (auto i = 0U; i < threads_number; ++i) {
        threads[i] = std::thread(function, i);
    }

    if (tail > 0U) {
        function(threads_number);
    }

    for (auto i = 0U; i < threads_number; ++i) {
        threads[i].join();
    }
}

