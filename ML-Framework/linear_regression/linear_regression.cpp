#include <linear_regression.h>
#include <matrix_math.h>
#include <ml_math.h>

matrix linear_regression::batch_gradient_fit(
    const matrix &x,
    const matrix &y,
    double learning_rate,
    double regularization_parameter,
    uint64_t number_of_epoches,
    cost_function_enum cost_function_type,
    int (*on_epoch)(double, uint64_t, const matrix &, matrix &, matrix &, double, double, uint64_t)
)
{
    uint64_t m = x.rows();
    uint64_t n = x.columns();

    // Initialize model weights to 0 (dimension: n x 1)
    matrix model(n, 1);
    for (uint64_t i = 0; i < n; ++i)
    {
        model[i][0] = 0.0;
    }

    matrix x_t = x.as_transposed();

    for (uint64_t epoch = 1; epoch <= number_of_epoches; ++epoch)
    {
        matrix y_hat = x * model;
        double cost = ml_math::mean_squared_error(const_cast<matrix&>(y), y_hat);

        if (on_epoch != nullptr)
        {
            int keep_going = on_epoch(cost, epoch, x, const_cast<matrix&>(y), model, learning_rate, regularization_parameter, number_of_epoches);
            if (!keep_going) break;
        }

        matrix error = y_hat - y;
        matrix gradient = (x_t * error) * (1.0 / m);
        model = model - (learning_rate * gradient);
    }

    return model;
}
