"""
Logical Errors to fix:
1) Multiple max degree!
If we have multiple max values in a degree matrix we need to 
search through all of them and not only use one (like right now).
It means that you can get ground_node with better IR value!

Ideas to imprive:
1) Use Hilbert curve instad of usual flatting
"""

import numpy as np

class PIG:
    def __init__(self, image, beta=95, mode='manual', stop=1e-5, verbose=False):
        """
        Args:
            image: np.array or PIL.Image
            beta: parameter of inner kernal function for image to graph translation
            mode: {'manual/onerun/usestop'} mode of a PIG run, see _autorun for details 
            stop: stop parameter for recursion
            verbose: print of progress info
        """
        # INNER
        self._width = image.shape[0]
        self._hight = image.shape[1]
        self.beta = beta
        
        self.flat_image = image.flatten()
        self.classes = np.zeros(self.__len__())
        
        # Run pig on auto
        self._autorun(mode, verbose)

        print('\nPIG initialization is done.')


    def _autorun(self, mode, verbose=False):
        """Run of PIG
        
        Modes:
            manual: user run all functions by himself
            onerun: auto run of one cycle
            usestop: auto run of recursion that use stop condition
                     stop condition must be specified!
        """
        
        if mode == 'manual':
            return
        
        elif mode == 'usestop':
            self.weights = self._compute_weights(self.beta)
            self.degree = self._compute_degree()
            
            max_degree_idxs = np.where(
                self.degree == self.degree.max())
            
            self.ground_node = self.degree.argmax()
            
        elif mode == 'onerun':
            # 1.1. Generate weights
            if verbose: print('Graph weights generation...')
            self.weights = self._compute_weights(self.beta)

            # 1.2. Calculate graph degree
            if verbose: print('Graph degree calculation...')
            self.degree = self._compute_degree()

            # 2.1. Determent ground node
            if verbose: print('Graph ground node determining ...')
            self.ground_node = self.degree.argmax()

            # 2.2. Build Laplacian matrix
            if verbose: print('Laplacian matrix buildig...')
            self.laplace = self._build_laplace()
            
            # 2.3 Droping ground node from degree and Laplacian matrix
            if verbose: print('Droping ground node from laplace and degree...')
            self._drop_ground_node()
            
            # 3.0. Solve L*x = d for x
            if verbose: print('Solving itself...')
            self._solve_itself()
            
            # 4.0 Find best threshold based on Isometric ratio
            if verbose: print('Finding best threshold based on Isometric ratio...')
            self._find_best_threshold(verbose=verbose)
            
            # Assign classes to each pixel based on best thresholding 
            if verbose: print('Assigning classes to each pixel...')
            self._absorb_classes()
        else:
            raise ValueError('Arg \'mode\' must be one of: manual, onerun, usestop')

    def _absorb_classes(self, ground_to='in'):
        """Separate nodes to classes
        
        ground_to - {in, out} witch class assign to ground node, 
                    None for -1 class 
        """
        # Find number of new class for inside and outside set
        currnt_classes = np.unique(self.classes)
        new_class_in = currnt_classes.max() + 1
        new_class_out = currnt_classes.max() + 2
        
        # Ground node to new_class
        if ground_to == 'in':
            self.classes[self.ground_node] = new_class_in
        elif ground_to == 'out':
            self.classes[self.ground_node] = new_class_out
        else:
            self.classes[self.ground_node] = -1
        
        cond_out = self.x[:self.ground_node] > self.threshold
        self.classes[:self.ground_node][cond_out] = new_class_out
        cond_out = self.x[self.ground_node:] > self.threshold
        self.classes[self.ground_node+1:][cond_out] = new_class_out
        
        cond_in = self.x[self.ground_node:] <= self.threshold
        self.classes[self.ground_node+1:][cond_in] = new_class_in
        cond_in = self.x[:self.ground_node] <= self.threshold
        self.classes[:self.ground_node][cond_in] = new_class_in


    def _find_best_threshold(self, num_of_steps=20, verbose=False):
        """Greedy threshold search
        
        Best threshold for lowest Isometric Ratio
        """
        xmin = self.x.min()
        xmax = self.x.max()
        step = (xmax - xmin)/num_of_steps
        
        lower_th = None
        lower_IR = 1

        # for each potential threshold
        for threshold in np.arange(xmin+step, xmax, step):
            IR = self._compute_isometric_ratio(threshold)
            
            if IR < lower_IR:
                lower_IR = IR
                lower_th = threshold
                
        self.threshold = lower_th
        if verbose:
            print(f'\tThreshold:\t\t{lower_th}\n\tIsometric Ratio:\t{lower_IR}')


    def _compute_isometric_ratio(self, threshold):
        """Compute Isometric Ratio of a graph
        
        Isometric Ratio - sum of edge weights divided by
        volume of one of two sets(graphs) seporated by threshold
        """
        perimeter = 0 # dS
        volume = 0

        # A - inside set; B - outside set
        set_A = self.x >= threshold
        set_B = self.x < threshold
        
        for a_idx in np.where(set_A)[0]:
            # Compute inside set volume
            volume += self.degree[a_idx]

            for b_idx in np.where(set_B)[0]:
                # Compute boundaey betwen inside and outside sets
                perimeter += self.weights[a_idx, b_idx]
        
        # Compute isometric ratio
        iso_ratio = perimeter / volume
        return iso_ratio


    def _solve_itself(self):
        self.x = np.linalg.solve(
            self.laplace, self.degree)
        return self.x


    def _drop_ground_node(self):
        """Drop ground node from Laplace and degree
        
        ground node - vertex with max value in degree
        """
        self.laplace =  np.concatenate((
            np.concatenate((
                self.laplace[:self.ground_node,:self.ground_node],
                self.laplace[:self.ground_node,self.ground_node+1:]),1),

            np.concatenate((
                self.laplace[self.ground_node+1:,:self.ground_node],
                self.laplace[self.ground_node+1:,self.ground_node+1:]), 1)))

        self.degree = np.concatenate((
            self.degree[:self.ground_node], 
            self.degree[self.ground_node+1:]))


    def _build_laplace(self):
        L = -1 * self.weights.copy()
        for i in range(self.degree.size):
            L[i,i] = self.degree[i]
        return L 


    def _compute_degree(self):
        """Return degree of a graph
        
        For each vertex: Sum of weights of all edges connected 
        to this vertex (excluding itself)
        """
        N = self.__len__()
        # Allocate memory
        D = np.zeros(N)

        for i in range(N):
            # - weights[i,i] because pixel are not connected to itself
            D[i] = self.weights[i].sum() - self.weights[i,i]
        return D


    def _kernel(self, x1, x2, beta=1):
        """Basic kernel (read kernel trick)"""
        d = (x1 - x2)**2
        return np.exp(-beta * d)


    def _compute_weights(self, beta=1):
        """Return weights of a graph edges generated for image
        
        Use _kernal function for similarity measure
        """
        N = self.__len__()
        # Allocate memory
        W = np.zeros((N,N), dtype='float')
        
        for i in range(N):
            for j in range(N):
                if i > j:
                    W[i,j] = self._kernel(self.flat_image[i],
                                         self.flat_image[j], 
                                         beta)
        W += W.T            
        for i in range(N):
            W[i,i] = 1
        
        return W


    def __len__(self):
        """Return number of nodes in a graph (read pixels)"""
        return self.flat_image.size