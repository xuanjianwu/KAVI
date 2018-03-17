; Testing

(define (domain Blockworld )
    (:requirements :strips :typing :negative-preconditions)
    (:types
        Robot Box - object
    )
    (:predicates
        (holding ?a - Robot ?b - Box)
        (onground ?a - Box)
        (on ?a - Box ?b - Box)
        (clear ?a - Box)
        (empty ?a - Robot)
    )
    (:action stack
        :parameters (?R1 - Robot ?B1 - Box ?B2 - Box )
        :precondition (and 
            (holding ?R1 ?B2)
            (clear ?B1)
            
        )
        :effect (and 
            (on ?B2 ?B1)
            (empty ?R1)
            (clear ?B2)
            (not (holding ?R1 ?B2))
            (not (clear ?B1))
        )
    )
)
