import numpy as np
from matplotlib import pyplot as plt

def discrete_probability_random_number_generator(numbers, probabilities, shape):

    if np.size(numbers) != np.size(probabilities):
        raise Exception('Error: size of numbers and probabilities must be the same')
    if np.size(numbers) == 0:
        raise Exception('Error: size of numbers must be the greater than zero')
    if np.size(probabilities) == 0:
        raise Exception('Error: size of probabilities must be the greater than zero')
    if np.any(np.less(probabilities, 0)):
        raise Exception('Error: Probabilities must be greater than or equal to zero')

    n = np.product(shape)
    uniform_n = 1000

    upper_bounds = np.zeros(len(probabilities)+1)
    for i in range(len(upper_bounds)):
        upper_bounds[i] = sum(probabilities[0:i])*uniform_n

    random_output = np.random.randint(0, uniform_n, n)
    # output = np.zeros(n)
    #
    # for i in range(n):
    #     for k in range(1, len(upper_bounds)):
    #         if (random_output[i] >= upper_bounds[k-1]) and (random_output[i] < upper_bounds[k]):
    #             output[i] = numbers[k-1]
    # return output

    output_bound = np.zeros([len(upper_bounds), n])
    for k in range(1, len(upper_bounds)):
        # k = np.linspace(1, len(upper_bounds), len(upper_bounds))

        output_bound[k, ] = np.logical_and(np.greater_equal(random_output, upper_bounds[k-1]),
                                           np.less(random_output, upper_bounds[k]))
    allrands = np.sum(output_bound[1:, ]*(np.reshape(numbers, [len(numbers), 1]) @ np.ones([1, n])), 0)
    return np.reshape(allrands, shape)

randoms = discrete_probability_random_number_generator([-1, 1], [0.5, 0.5], [100, 5])
print(randoms)

hist = np.histogram(randoms)

plt.figure()
plt.hist(randoms, bins=2)
plt.show()