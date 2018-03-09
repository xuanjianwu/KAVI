(define (problem fourBoxes)
    (:domain Blockworld )
    (:objects
         B2 B1 - Box
         R1 - Robot
    )
    (:init
        (holding R1 B2)
        (clear B1)
    )
    (:goal
        (and 
            (empty R1)
            (on B2 B1)
        )
    )
)