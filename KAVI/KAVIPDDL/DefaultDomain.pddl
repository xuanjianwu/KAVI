; Testing

(define (domain Blockworld )
    (:requirements :strips :typing :negative-preconditions)
    (:types
        Robot Box - object
    )
    (:predicates
        (onground ?a - Box)
        (clear ?a - Box)
        (empty ?a - Robot)
        (holding ?a - Robot ?b - Box)
        (on ?a - Box ?b - Box)
    )
    (:action stack
        :parameters (?R1 - Robot ?B1 - Box ?B2 - Box )
        :precondition (and 
            (clear ?B1)
            (holding ?R1 ?B2)
            
        )
        :effect (and 
            (on ?B2 ?B1)
            (empty ?R1)
            (clear ?B2)
            (not (clear ?B1))
            (not (holding ?R1 ?B2))
        )
    )
)
