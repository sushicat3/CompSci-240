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

### Implementation Details

In this implementation, the algorithm classifies cities, based on whether or not they contain certain letters.

Inital data given is 3 lists of cities. 1 list of 100 US cities, 1 list of 100 Russian cities, and one list of other cities (cities that are neither US nor Russian)
	
	training/
		usCities100.txt
		russiaCities100.txt
		otherCities100.txt

The second set of data is 3 more lists of cities with  "unknown" classifications. 
	
	unknown/
		usCitiesNext50.txt
		russiaCitiesNext50.txt
		otherCitiesNext50.txt

We know their classifcations but the algorithm doesn't, and it will try to classify them correctly. Sometimes it will get it wrong, so we will output the **accuracy** of the NBC algorithm's classifications.
	
	accuracy % = [correct trials] / [total trials] * 100 %

This implementation will run 3 "Experiments" of the NBC algorithm. 

In the first "Experiment" the algorithm uses the 100 Russian cities and 100 US cities as training data. Then, it classifies the next 50 Russian cities as either US or Russian, and the next 50 US cities as either US or Russian.

The second and third "Experiments" are exactly like the first, but they will use Russian and Other, and Other and US cities, respectively.

What are the predictors? In this implementation they will be the existence of certain letters in the city's name.

There are 26 predictors that make up the set.

	X = {Xa, Xb, ..., Xz}

Basically, if a city contains letters that are similar to letters commonly found in the names of a certain type of city, it will be classified as that type of city.
