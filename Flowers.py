import numpy as np
from sklearn import datasets
import matplotlib.pyplot as plt
     
#Load and prepare data

iris = datasets.load_iris()
X = iris["data"][:, (2, 3)].astype(float)   # petal length, petal width
d = (iris["target"] == 0).astype(int)       # 1 = Setosa, 0 = non-Setosa

N, D = X.shape

# Shuffle once
rng = np.random.default_rng(42)
perm = rng.permutation(N)
X = X[perm]
d = d[perm]
     
#Plot input space

plt.figure()
plt.grid(True)
plt.plot(X[d == 0, 0], X[d == 0, 1], "o", label="d=0 (non-Setosa)")
plt.plot(X[d == 1, 0], X[d == 1, 1], "x", label="d=1 (Setosa)")
plt.xlabel("Petal Length")
plt.ylabel("Petal Width")
plt.title("Iris Input Space")
plt.legend()
plt.show()


def step(net: float) -> int:

    """
    Binary step activation function.

    Input:
        net : net input to the neuron (scalar)

    Output:
        y : binary output (0 or 1)
    """
    return 1 if net >= 0 else 0


def predict_all(X: np.ndarray, w: np.ndarray) -> np.ndarray:
    """
    Predict class labels for all samples.

    Notes:
        - The bias is included as w[0].
        - Inputs are augmented as x_aug = [1, x1, x2].
        - Net input: net = w^T x_aug
    """
    N = X.shape[0]
    X_aug = np.hstack([np.ones((N, 1)), X])  # shape (N, 3)
    nets = X_aug @ w
    return np.array([step(v) for v in nets], dtype=int)


def accuracy(y_hat: np.ndarray, y_true: np.ndarray) -> float:
    """
    Compute classification accuracy.
    """
    return float(np.mean(y_hat == y_true))
     
#TODO : Perceptron learning rule

def perceptron_update(
    x_i: np.ndarray,
    d_i: int,
    w: np.ndarray,
    eta: float
):
    """Perform ONE perceptron update using a single training sample.

    Inputs:
        x_i : shape (2,) input vector = [x1, x2]
        d_i : desired output (0 or 1)
        w   : current weight vector INCLUDING bias, shape (3,)
              w[0] is the bias weight
              w[1] corresponds to x1, w[2] corresponds to x2
        eta : learning rate

    Outputs:
        w_new : updated weight vector, shape (3,)
        error : calculated error
    """
    error = d_i - predict_all(x_i.reshape(1, -1), w)[0]
    w_new = w + eta * error * np.hstack(([1], x_i))
    return w_new, error

#TODO : Training loop

def train_perceptron(
    X: np.ndarray,
    d: np.ndarray,
    eta: float,
    max_steps: int
):
    """
    Train a perceptron using the perceptron learning rule.

    NOTE:
      - Training is organized by STEPS (iterations), not epochs.
      - At step t, one training sample is selected).
      - The bias is included as part of the weight vector.
      - Plot step/iteration vs error to monitor the training

    Inputs:
        X         : shape (N,2) input matrix
        d         : shape (N,) desired outputs (0 or 1)
        eta       : learning rate
        max_steps : maximum number of update steps/iterations

    Outputs:
        w : learned weight vector INCLUDING bias, shape (3,)
            w[0] is the bias weight (multiplies constant input 1)
            w[1] corresponds to x1
            w[2] corresponds to x2
    """

    N, D = X.shape
    assert D == 2

    # Initialize weights (including bias as w[0])
    w = np.zeros(D + 1)

    for step in range(max_steps):
        # Select one training sample (cyclically)
        i = step % N
        x_i = X[i]
        d_i = d[i]

        # Perform perceptron update
        w, error = perceptron_update(x_i, d_i, w, eta)
        
        print(f"Step {step+1}, Sample {i}, Weights: {w}, Error: {error}")

    return w
     
#Run training

eta = 0.1
max_steps = 500

w = train_perceptron(X, d, eta, max_steps)
     
#Evaluate performance

y_hat = predict_all(X, w)
acc = accuracy(y_hat, d)

print(f"Final training accuracy: {acc:.4f}")
print(f"Final weights (bias included as w[0]): w = {w}")
     
#Decision boundary

print("\nDecision boundary:")
print("w0*1 + w1*x1 + w2*x2 = 0")
print(f"{w[0]:.6f}*1 + {w[1]:.6f}*x1 + {w[2]:.6f}*x2 = 0")
     
#TODO : Plot decision boundary
def plot_decision_boundary(X: np.ndarray, w: np.ndarray, d: np.ndarray = d):
    plt.figure()
    plt.grid(True)
    # Plot data points
    plt.plot(X[d == 0, 0], X[d == 0, 1], "o", label="d=0 (non-Setosa)")
    plt.plot(X[d == 1, 0], X[d == 1, 1], "x", label="d=1 (Setosa)")
    plt.xlabel("Petal Length")
    plt.ylabel("Petal Width")
    plt.title("Perceptron Decision Boundary")
    # Calculate decision boundary line
    x1_vals = np.array(plt.gca().get_xlim())
    x2_vals = -(w[0] + w[1] * x1_vals) / w[2]
    # Plot decision boundary
    plt.plot(x1_vals, x2_vals, 'r--', label='Decision Boundary')
    plt.legend()
    plt.show()
plot_decision_boundary(X, w)

# TODO (students):
# Plot the training samples and the perceptron decision boundary.
#
# Requirements:
#   - Plot the two classes in the input space (petal length vs. petal width).
#   - Use the learned weights and bias to plot the decision boundary.
#   - Clearly label axes, legend, and title.
