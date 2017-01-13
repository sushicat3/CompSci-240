Naive Bayes Classification
===========

### Overview

People always like to classify things, and NBC can be very fast compared to other classification algorithms. NBC uses the **Bayes Theorem**. It generates a "hypothesis" which is used to classify an object based on a set of "predictors."

A "predictor" is simply a piece of observed data about that object. We can define a set of predictors and then any object will have a subset of the predictors.
	
	Xi = predictor
	X  = set of predictors

The classification algorithm is "naive" because it assumes that all of the predictors we've defined are **independent**--observing one predictor doesn't affect the existence of another predictor. 
	
	P(Xi and Xj|C) = P(Xi|C) * P(Xj|C)

A "prior" is the probability an object has a particular classification.

	P(C) = prior

A "likelihood" is the probability that an object of a particular classification has a particular predictor.
	
	P(Xi|C) = likelihood

A "posteriori" is the probability than an object with a particuar set of predictors has a particular classification. 	
	
	P(C|Xi and Xj) = posteriori

The posteriori is used to make the hypothesis. It can be computed using Bayes Theorem:
	
	P(C|Xi and Xj) = [P(Xi|C) * P(Xj|C) * P(C)] / P(X)
	where X = {Xi, Xj}

	or

	P(C| Xi and not Xj) = [P(Xi|C) * P(not Xj|C) * P(C)] / P(X)
	where not Xj means that predictor was not observed

As you can see the posteriori calculation uses the prior and the likelihoods of a particular classification C. 

A posteriori is calculated for each different classification (suppose they are A, B and C). Then the classification that generated the maximum posteriori is the "hypothesis" for that object.
	
	observed predictors: Xj
	P(A|not Xi and Xj) = [P(not Xi|A) * P(Xj|A) * P(A)] / P(X) 
	P(B|not Xi and Xj) = [P(not Xi|B) * P(Xj|B) * P(B)] / P(X) < maximum, algorithm hypothesizes that object classification is B
	P(C|not Xi and Xj) = [P(not Xi|C) * P(Xj|C) * P(C)] / P(X) 

### Algorithm

#### Step 1: Training

The algorithm starts with a set of already classified objects. It observes priors and predictors from that data. Then it calculates likelihoods for each predictor and classification.

#### Step 2: Classifying

The algorithm generates the MAP (maximum posteriori) hypothesis for each obejct, and uses it to classify them.
