(declare-const a1 Real)
(assert (<= a1 2))
(assert (>= a1 -2))
(declare-const a2 Real)
(assert (<= a2 2))
(assert (>= a2 -2))
(declare-const a3 Real)
(assert (<= a3 2))
(assert (>= a3 -2))
(declare-const a4 Real)
(assert (<= a4 2))
(assert (>= a4 -2))
(declare-const b12 Real)
(assert (<= b12 1))
(assert (>= b12 -1))
(declare-const b13 Real)
(assert (<= b13 1))
(assert (>= b13 -1))
(declare-const b14 Real)
(assert (<= b14 1))
(assert (>= b14 -1))
(declare-const b23 Real)
(assert (<= b23 1))
(assert (>= b23 -1))
(declare-const b24 Real)
(assert (<= b24 1))
(assert (>= b24 -1))
(declare-const b34 Real)
(assert (<= b34 1))
(assert (>= b34 -1))

(declare-const c1 Int)
(assert (or (= c1 1) (= c1 -1) ) )
(declare-const c2 Int)
(assert (or (= c2 1) (= c2 -1) ) )
(declare-const c3 Int)
(assert (or (= c3 1) (= c3 -1) ) )
(declare-const c4 Int)
(assert (or (= c4 1) (= c4 -1) ) )
(declare-const c5 Int)
(assert (or (= c5 1) (= c5 -1) ) )
(declare-const c6 Int)
(assert (or (= c6 1) (= c6 -1) ) )
(declare-const c7 Int)
(assert (or (= c7 1) (= c7 -1) ) )
(declare-const c8 Int)
(assert (or (= c8 1) (= c8 -1) ) )

(define-fun ising4((q1 Real)(q2 Real)(q3 Real)(q4 Real))Real
(+ (* a1 q1) (* a2 q2) (* a3 q3) (* a4 q4) (* b12 q1 q2) (* b13 q1 q3) (* b14 q1 q4) (* b23 q2 q3) (* b24 q2 q4) (* b34 q3 q4))
)

(define-fun assist4((q1 Real)(q2 Real)(q3 Real))Real
(+ (* c1 (/ (- 1 q1) 2) (/ (- 1 q2) 2) (/ (- 1 q3) 2)) (* c2 (/ (+ 1 q1) 2) (/ (- 1 q2) 2) (/ (- 1 q3) 2)) (* c3 (/ (- 1 q1) 2) (/ (+ 1 q2) 2) (/ (- 1 q3) 2)) (* c4 (/ (+ 1 q1) 2) (/ (+ 1 q2) 2) (/ (- 1 q3) 2)) (* c5 (/ (- 1 q1) 2) (/ (- 1 q2) 2) (/ (+ 1 q3) 2)) (* c6 (/ (+ 1 q1) 2) (/ (- 1 q2) 2) (/ (+ 1 q3) 2)) (* c7 (/ (- 1 q1) 2) (/ (+ 1 q2) 2) (/ (+ 1 q3) 2)) (* c8 (/ (+ 1 q1) 2) (/ (+ 1 q2) 2) (/ (+ 1 q3) 2))))


(assert (= (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 1 -1 -1 (assist4 1 -1 -1)) (ising4 -1 1 1 (assist4 -1 1 1)) (ising4 1 1 -1 (assist4 1 1 -1))) )
(assert (<= (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 -1 -1 -1 -1)) )
(assert (<= (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 1 -1 -1 -1)) )
(assert (<= (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 1 1 -1 -1)) )
(assert (<= (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 -1 1 1 -1)) )
(assert (<= (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 -1 -1 -1 1)) )
(assert (<= (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 1 -1 -1 1)) )
(assert (<= (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 1 1 -1 1)) )
(assert (<= (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 -1 1 1 1)) )
(assert (< (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 -1 1 -1 -1)) )
(assert (< (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 -1 -1 1 -1)) )
(assert (< (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 1 -1 1 -1)) )
(assert (< (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 1 1 1 -1)) )
(assert (< (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 -1 1 -1 1)) )
(assert (< (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 -1 -1 1 1)) )
(assert (< (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 1 -1 1 1)) )
(assert (< (ising4 -1 -1 -1 (assist4 -1 -1 -1)) (ising4 1 1 1 1)) )

