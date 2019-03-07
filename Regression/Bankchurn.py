#!/usr/bin/env python
# coding: utf-8

# In[5]:


import numpy as np # For data manipulation
import pandas as pd # For data representation
import matplotlib.pyplot as plt # For basic visualization
import seaborn as sns  # For synthetic visualization
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier # K neighbors classification model
from sklearn.naive_bayes import GaussianNB # Gaussian Naive bayes classification model
from sklearn.svm import SVC # Support Vector Classifier model
from sklearn.tree import DecisionTreeClassifier # Decision Tree Classifier model
from sklearn.linear_model import LogisticRegression # Logistic Regression model
from sklearn.ensemble import RandomForestClassifier # Random Forest Classifier model
from sklearn.metrics import accuracy_score # For checking the accuracy of the model


# In[7]:


# Importing the dataset
churn_dataset = pd.read_csv('C:/Users/Vijay/Desktop/Aj/Data Science/bankchurn/Churn_Modelling.csv')
# Visualizing first five elements in the dataset
churn_dataset.head()


# In[9]:


# Checking basic information (rows, columns, missing values, datatypes of columns, etc) in our dataset
churn_dataset.info()


# In[11]:


# Checking statistical information in our dataset
churn_dataset.describe()


# In[16]:


# Checking set of unique characters in each categorical feature
for col in churn_dataset.columns:  # Looping over all columns 
    if churn_dataset[col].dtypes == 'object':
        num_of_unique_cat = len(churn_dataset[col].unique()) # Checking the length of unique characters
        print("feature '{col_name}' has '{unique_cat}' unique categories".format(col_name = col, unique_cat=num_of_unique_cat))


# In[15]:


# Deleting the Surname feature from the dataset
churn_dataset = churn_dataset.drop("Surname", axis=1)


# In[19]:


# Creating a pivot table demonstrating the percentile
# Of different genders and geographical regions in exiting the bank 
visualization_1 = churn_dataset.pivot_table("Exited", index="Gender", columns="Geography")
visualization_1


# In[20]:


# Deleting gender and geography features from the dataset
churn_dataset = churn_dataset.drop(["Geography", "Gender"], axis=1)


# In[22]:


churn_dataset = churn_dataset.drop(["RowNumber", "CustomerId"], axis=1)


# In[24]:


correlation = churn_dataset.corr()
sns.heatmap(correlation.T, square=True, annot=False, fmt="d", cbar=True)


# In[26]:


# Shuffling the dataset
churn_dataset = churn_dataset.reindex(np.random.permutation(churn_dataset.index))


# In[29]:


data = churn_dataset.drop("Exited", axis=1)
target = churn_dataset["Exited"]


# In[31]:


# Splitting feature data and target into training and testing
X_train, X_test, y_train, y_test = train_test_split(data, target)


# In[33]:


# Creating a python list containing all defined models
model = [GaussianNB(), KNeighborsClassifier(), SVC(), DecisionTreeClassifier(), RandomForestClassifier(n_estimators=5, random_state=0), LogisticRegression()]
model_names = ["Gaussian Naive bayes", "K-nearest neighbors", "Support vector classifier", "Decision tree classifier", "Random Forest", "Logistic Regression",]
for i in range(0, 6):
    y_pred = model[i].fit(X_train, y_train).predict(X_test)
    accuracy = accuracy_score(y_pred, y_test)*100
    print(model_names[i], ":", accuracy, "%")


# In[44]:


# Working with the selected model

model = RandomForestClassifier(n_estimators = 100, random_state = 5)
y_pred = model.fit(X_train, y_train).predict(X_test)
print("Our accuracy is:", accuracy_score(y_pred, y_test)*100, "%")


# In[63]:


print("Enter the credit score of the client \n")
credit_score = int(input())
print("Enter the age of the client \n")
age = int(input())
print("Enter the tenure of the client \n")
tenure = int(input())
print("Enter the current balance of the client \n")
balance = float(input())
print("Enter the number of product the client use \n")
product_no = int(input())
print("Press 1 if the user has a credit card or 0 if not \n")
credit_card = int(input())
print("Press 1 if the user is an active member or 0 if not \n")
active_member = int(input())
print("Enter the estimated salary of the client \n")
salary = float(input())


X_user = np.array([678,27,10,1020,1,1,100000])

y_pred = model.predict([X_user])
index = y_pred  

print(index)
if index == 1:
   print("\n Client is not exiting the bank")
   elif index == 0:
      print("\n Client is on the threshold of exiting the bank")
 print("\n Consider taking further steps to incentivise the client")


# In[82]:



X_user = np.array([600,50,4,0000,1,1,1,100000])

y_pred = model.predict([X_user])
index = y_pred  

print(index)

