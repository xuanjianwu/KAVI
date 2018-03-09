; Testing

(define (domain Blockworld )
    (:requirements :strips :typing :negative-preconditions)
    (:types
        Robot Box - object
    )
    (:predicates
        (holding ?a - Robot ?b - Box)
        (empty ?a - Robot)
        (on ?a - Box ?b - Box)
        (onground ?a - Box)
        (clear ?a - Box)
    )
    (:action stack
        :parameters (?R1 - Robot ?B1 - Box ?B2 - Box )
        :precondition (and 
            (clear ?B1)
            (not (holding ?R1 ?B2))
        )
        :effect (and 
            (empty ?R1)
            (on ?B2 ?B1)
            (clear ?B2)
            
        )
    )
)
