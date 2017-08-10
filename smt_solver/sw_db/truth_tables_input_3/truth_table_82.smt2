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

(define-fun ising4((q1 Real)(q2 Real)(q3 Real)(q4 Real))Real
(+ (* a1 q1) (* a2 q2) (* a3 q3) (* a4 q4) (* b12 q1 q2) (* b13 q1 q3) (* b14 q1 q4) (* b23 q2 q3) (* b24 q2 q4) (* b34 q3 q4))
)


(assert (= (ising4 -1 -1 -1 -1) (ising4 1 -1 -1 1) (ising4 -1 1 -1 -1) (ising4 1 1 -1 -1) (ising4 -1 -1 1 1) (ising4 1 -1 1 -1) (ising4 -1 1 1 1) (ising4 1 1 1 -1)) )
(assert (< (ising4 -1 -1 -1 -1) (ising4 1 -1 -1 -1)) )
(assert (< (ising4 -1 -1 -1 -1) (ising4 -1 -1 1 -1)) )
(assert (< (ising4 -1 -1 -1 -1) (ising4 -1 1 1 -1)) )
(assert (< (ising4 -1 -1 -1 -1) (ising4 -1 -1 -1 1)) )
(assert (< (ising4 -1 -1 -1 -1) (ising4 -1 1 -1 1)) )
(assert (< (ising4 -1 -1 -1 -1) (ising4 1 1 -1 1)) )
(assert (< (ising4 -1 -1 -1 -1) (ising4 1 -1 1 1)) )
(assert (< (ising4 -1 -1 -1 -1) (ising4 1 1 1 1)) )

