# Approximating Likelihood Ratios with Calibrated Discriminative Classifiers

https://arxiv.org/abs/1506.02169

* Kyle Cranmer
* Juan Pavez
* Gilles Louppe

In many fields of science, generalized likelihood ratio tests are established tools for
statistical inference.
 At the same time, it has become increasingly common that a simulator (or
generative model) is used to describe complex processes that tie parameters
$\theta$ of an underlying theory and measurement apparatus to high-dimensional
observations $\mathbf{x}\in \mathbb{R}^p$. However, simulator often do not provide a way to evaluate
the likelihood function for a given observation $\mathbf{x}$, which motivates a new class of likelihood-free
inference algorithms.
In this paper, we show that likelihood ratios are invariant under a specific class of dimensionality reduction maps $\mathbb{R}^p \mapsto \mathbb{R}$.  As a direct consequence, we show that discriminative classifiers can be used to
approximate the generalized likelihood ratio statistic when only a generative model for the data is
available. This leads to a new machine learning-based approach to likelihood-free inference that is complementary to Approximate Bayesian Computation, and which does not require a prior on the model parameters. Experimental results
on artificial problems with known exact likelihoods illustrate the potential of the proposed method.

