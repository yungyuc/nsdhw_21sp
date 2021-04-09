import numpy as np

class PIG:
    def __init__(self, image, beta=95):
        # INNER
        self._width = image.shape[0]
        self._hight = image.shape[1]
        
        self.flat_image = image.flatten()
        self.classes = np.zeros(self.__len__())
        
        # API
        # 1.1. Generate weights
        self.weights = self._compute_weights(beta)
        
        # 1.2. Calculate graph degree
        self.degree = self._compute_degree()
        
        # 2.1. Determent ground node
        self.ground_node = self.degree.argmax()
        
        # 2.2. Build Laplacian matrix
        self.laplace = self._build_laplace()
        
        print('PIG is done.')

    def absorb_classes(self):
        #!!! FININSH
        currnt_classes = np.unique(self.classes)
        next_class = currnt_classes.max() + 1
        
        self.classes[self.ground_node] = next_class
        self.classes[]
        
        
    def _find_best_threshold(self, num_of_steps=20):
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
        print(f'Threshold: {lower_th} | Isometric Ratio: {lower_IR}')


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


    def solve_self(self):
        self.x = np.linalg.solve(
            self.laplace, self.degree)
        return self.x


    def drop_ground_node(self):
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