(define (problem fourBoxes)
    (:domain Blockworld )
    (:objects
         D C B A - Box
         Rob - Robot
    )
    (:init
        (clear A)
        (on B A)
        (on C B)
        (onground C)
        (empty Rob)
        (clear D)
        (onground D)
    )
    (:goal
        (and 
            (on C B)
            (on D C)
        )
    )
)