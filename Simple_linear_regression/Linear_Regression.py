# %%
import torch
import torch.nn as nn
import numpy as np
from torchvision import datasets, transforms
import matplotlib.pyplot as plt
from pathlib import Path
##preparing and loading dataset
#create known parameters
weight = 0.7
bias = 0.3

start =0
end =1
step =0.02
X=torch.arange(start,end,step).unsqueeze(dim=1)
y=weight*X + bias
print(X[:10],y[:10]) #checking first 10 values of X and y
print(len(X),len(y)) #checking length of X and y
#splitting data into training and test sets
train_split = int(0.8*len(X))
X_train, y_train = X[:train_split], y[:train_split]
X_test, y_test = X[train_split:], y[train_split:]
#plotting the data

def plot_predictions(train_data=X_train,
                     train_labels=y_train,
                        test_data=X_test,
                        test_labels=y_test,
                        prediction = None):
    plt.figure(figsize=(10,7))
    plt.scatter(train_data,train_labels,c="b",s=4,label="Training data")
    plt.scatter(test_data,test_labels,c="g",s=4,label="Testing data")
    if prediction is not None:
        plt.scatter(test_data,prediction,c="r",s=4,label="Predictions")
        plt.legend(prop= {"size":14})
plot_predictions()

#building linear regression model
class LinearRegressionModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.weight = nn.Parameter(torch.randn(1,requires_grad=True,dtype=torch.float))
        self.bias = nn.Parameter(torch.randn(1,requires_grad=True,dtype=torch.float))

        #forward method to define computation in the model
    def forward(self,x:torch.Tensor)->torch.Tensor: #x is input data of type torch.Tensor
        return self.weight * x + self.bias

#create random seed
torch.manual_seed(42)

# create instance of the model
model_0 = LinearRegressionModel()
print(list(model_0.parameters()))
#list named parameters of the model
print(model_0.state_dict())
#making predictions using torch.inference mode

with torch.inference_mode():
    y_preds = model_0(X_test)
print(y_preds[:10])

#train model
#define loss function and optimizer(l1 and stochastic gradient descent)
loss_fn = nn.L1Loss()
optimizer = torch.optim.SGD(params=model_0.parameters(),lr=0.01)
#training loop
epochs =300 #loop through data 100 times
epoch_count = []
loss_values = []
test_loss_values = []
torch.manual_seed(42)
#loop through data
for epoch in range(epochs):
    #set model to training mode
    model_0.train() # sets all parameters that require gradients to True
    #1.forward pass
    y_pred = model_0(X_train)
    #2.calculate loss
    loss = loss_fn(y_pred,y_train)

    #3.zero gradients
    optimizer.zero_grad()

    #4.backward pass
    loss.backward()

    #5.update weights
    optimizer.step()


    model_0.eval()  # sets all parameters that require gradients to False

#testing the model
    with torch.inference_mode(): #turns off gradients
        test_pred = model_0(X_test)
        test_loss = loss_fn(test_pred,y_test)
    if epoch % 10 ==0:
        epoch_count.append(epoch)
        loss_values.append(loss.item())
        test_loss_values.append(test_loss.item())
        print(f"Epoch: {epoch} | Train loss: {loss.item():.4f} | Test loss: {test_loss.item():.4f}")
#plotting loss curves
plt.plot(epoch_count,loss_values,label="Train loss")
plt.plot(epoch_count,test_loss_values,label="Test loss")
plt.title("Loss curves")
plt.xlabel("Epochs")
plt.ylabel("Loss")
plt.legend()
print(f"Parameters after training: {model_0.state_dict()}")
#create model directory path
MODEL_PATH = Path("models")

MODEL_PATH.mkdir(parents=True,exist_ok=True)

#model save path
MODEL_NAME = "linear_regression_model_0.pth"
MODEL_SAVE_PATH = MODEL_PATH / MODEL_NAME
#save the model state dict
torch.save(obj=model_0.state_dict(),f=MODEL_SAVE_PATH)
print(f"Model saved to : {MODEL_SAVE_PATH}")
#%%