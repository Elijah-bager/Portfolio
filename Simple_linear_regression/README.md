# Number Classifier - Linear Regression with PyTorch

A simple PyTorch implementation of linear regression for number prediction.

## Description

This project demonstrates building and training a linear regression model using PyTorch. The model learns to predict output values based on a linear relationship: `y = 0.7*x + 0.3`.

The implementation includes:
- Data generation and train/test splitting
- Custom LinearRegressionModel class
- Training loop with loss tracking
- Model evaluation and visualization
- Model saving functionality

## Requirements

- Python 3.x
- PyTorch
- NumPy
- Matplotlib

## Installation

1. Clone or download this repository
2. Install required packages:
   ```bash
   pip install torch numpy matplotlib
   ```

## Usage

Run the main script to train the model:

```bash
python Linear_Regression.py
```

The script will:
1. Generate synthetic training data
2. Train the linear regression model for 300 epochs
3. Display training progress and loss curves
4. Save the trained model to `models/linear_regression_model_0.pth`

## Model Details

- **Architecture**: Simple linear layer (y = weight*x + bias)
- **Loss Function**: L1 Loss (Mean Absolute Error)
- **Optimizer**: Stochastic Gradient Descent (SGD) with learning rate 0.01
- **Training Epochs**: 300

## Files

- `Linear_Regression.py` - Main training script
- `models/linear_regression_model_0.pth` - Saved trained model weights
- `linear_regression_model.pth` - Additional saved model (root directory)
- `README.md` - This file

## Results

After training, the model should achieve low loss values and accurately predict the linear relationship between input and output values.
