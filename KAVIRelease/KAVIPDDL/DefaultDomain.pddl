; Testing

(define (domain Blockworld )
    (:requirements :strips :typing :negative-preconditions)
    (:types
        Robot Box - object
    )
    (:predicates
        (empty ?a - Robot)
        (clear ?a - Box)
        (on ?a - Box ?b - Box)
        (onground ?a - Box)
        (holding ?a - Robot ?b - Box)
    )
    (:action stack
        :parameters (?R1 - Robot ?B1 - Box ?B2 - Box )
        :precondition (and 
            (clear ?B1)
            (holding ?R1 ?B2)
            
        )
        :effect (and 
            (clear ?B2)
            (empty ?R1)
            (on ?B2 ?B1)
            (not (clear ?B1))
            (not (holding ?R1 ?B2))
        )
    )
)
