; Test

(define (domain Blockworld )
    (:requirements :strips :typing)
    (:types
        Robot Box - object
    )
    (:predicates
        (empty ?a - Robot)
        (clear ?a - Box)
        (onground ?a - Box)
        (on ?a - Box ?b - Box)
        (holding ?a - Robot ?b - Box)
    )
    (:action Stack
        :parameters (?R1 - Robot ?B1 - Box ?B2 - Box )
        :precondition (and 
            (clear ?B1)
            (not (holding ?R1 ?B2))
        )
        :effect (and 
            (clear ?B2)
            (empty ?R1)
            (on ?B2 ?B1)
        )
    )
)