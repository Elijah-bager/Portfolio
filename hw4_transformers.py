import numpy as np

# ---------------------------------------------------------------------
# TODO 1: Compute softmax
# ---------------------------------------------------------------------
def softmax(x):
    """Compute softmax values for each row of the input array."""
    exp_x = np.exp(x - np.max(x, axis=-1, keepdims=True))
    return exp_x / np.sum(exp_x, axis=-1, keepdims=True)


class TransformerBlock:
    # ---------------------------------------------------------------------
    # TODO 2: Initialize weights
    # ---------------------------------------------------------------------
    def __init__(self, d_model=4, num_heads=2, d_ff=8, d_k=2, seed=5526):
        """
        Initialize all weights and parameters for the transformer block.

        Args:
            d_model (int): Dimension of input features, i.e., D.
            num_heads (int): Number of attention heads.
            d_ff (int): Dimension of the feed-forward hidden layer.
            d_k (int): Dimension of Q/K/V for each head.
            seed (int): Random seed for reproducibility.
        """
        np.random.seed(seed)  # DO NOT change the seed value

        # Save parameters
        self.d_model = d_model
        self.num_heads = num_heads
        self.d_ff = d_ff
        self.d_k = d_k

        # You will initialize weights here (Wq, Wk, Wv, Wo, W1, b1, W2, b2)
        Wq_shape = (d_model, num_heads * d_k)
        Wk_shape = (d_model, num_heads * d_k)
        Wv_shape = (d_model, num_heads * d_k)
        Wo_shape = (num_heads * d_k, d_model)
        W1_shape = (d_model, d_ff)
        b1_shape = (d_ff,)
        W2_shape = (d_ff, d_model)
        b2_shape = (d_model,)
        self.Wq = np.random.randn(*Wq_shape) * 0.01
        self.Wk = np.random.randn(*Wk_shape) * 0.01
        self.Wv = np.random.randn(*Wv_shape) * 0.01
        self.Wo = np.random.randn(*Wo_shape) * 0.01
        self.W1 = np.random.randn(*W1_shape) * 0.01
        self.b1 = np.random.randn(*b1_shape) * 0.01
        self.W2 = np.random.randn(*W2_shape) * 0.01
        self.b2 = np.random.randn(*b2_shape) * 0.01

    # ---------------------------------------------------------------------
    # TODO 3: Implement scaled dot-product attention
    # ---------------------------------------------------------------------
    def scaled_dot_product_attention(self, Q, K, V):
        """
        Compute scaled dot-product attention.
        
        Args:
            Q, K, V: Query, Key, and Value matrices for a single head.
        Returns:
            output: The attention output matrix.
        """
        output = softmax((Q @ K.T) / np.sqrt(self.d_k)) @ V
        return output
        raise NotImplementedError("Implement the scaled dot-product attention.")

    # ---------------------------------------------------------------------
    # TODO 4: Implement multi-head attention
    # ---------------------------------------------------------------------
    def multi_head_attention(self, X):
        """
        Perform multi-head self-attention over the input sequence.

        Args:
            X: Input matrix of shape (num_tokens, d_model)
        Returns:
            Output of multi-head attention (same shape as X)
        """
        # Split the input into multiple heads
        Q = X @ self.Wq
        K = X @ self.Wk
        V = X @ self.Wv
        Y = np.zeros_like(X)
        for i in range(self.num_heads):
            Q_i = Q[:, i*self.d_k:(i+1)*self.d_k]
            K_i = K[:, i*self.d_k:(i+1)*self.d_k]
            V_i = V[:, i*self.d_k:(i+1)*self.d_k]
            Y += self.scaled_dot_product_attention(Q_i, K_i, V_i) @ self.Wo[i*self.d_k:(i+1)*self.d_k, :]
        return Y
        raise NotImplementedError("Implement the multi-head attention.")

    # ---------------------------------------------------------------------
    # TODO 5: Implement feed-forward network
    # ---------------------------------------------------------------------
    def feed_forward(self, X):
        """
        Apply a two-layer feed-forward neural network with ReLU activation.

        Args:
            X: Input matrix (num_tokens, d_model)
        Returns:
            Output matrix (num_tokens, d_model)
        """
        output = np.maximum(0, X @ self.W1 + self.b1) @ self.W2 + self.b2
        return output
        raise NotImplementedError("Implement the feed-forward network.")

    # ---------------------------------------------------------------------
    # PROVIDED: Layer Normalization (Do NOT modify)
    # ---------------------------------------------------------------------
    def layer_norm(self, x, eps=1e-6):
        """
        Normalize each token vector to have mean 0 and variance 1.
        Provided for you — do not change.
        """
        mean = np.mean(x, axis=-1, keepdims=True)
        std = np.std(x, axis=-1, keepdims=True)
        return (x - mean) / (std + eps)

    # ---------------------------------------------------------------------
    # TODO 6: Combine everything into the forward pass
    # ---------------------------------------------------------------------
    def forward(self, X):
        """
        Run the full forward pass of the transformer encoder block.

        Steps:
            1. Apply multi-head attention
            2. Add & normalize (residual connection)
            3. Apply feed-forward layer
            4. Add & normalize again
        """
        # multi-head attention
        attn_output = self.multi_head_attention(X)
        # add & normalize
        X = self.layer_norm(X + attn_output)
        # feed-forward
        ff_output = self.feed_forward(X)
        # add & normalize
        output = self.layer_norm(X + ff_output)
        return output
        raise NotImplementedError("Implement the full transformer forward pass.")
        

# ---------------------------------------------------------------------
# FIXED INPUT (all students will use this)
# ---------------------------------------------------------------------
X = np.array([
    [0.5, 1.0, 0.3, 0.7],
    [0.2, 0.9, 0.8, 0.1],
    [0.6, 0.4, 0.5, 0.9]
])

# --- Run forward pass ---
model = TransformerBlock()
output = model.forward(X)
print(np.round(output, 3))