; Test

(define (domain Blockworld )
    (:requirements :strips :typing :negative-preconditions)
    (:types
        Robot Box - object
    )
    (:predicates
        (on ?a - Box ?b - Box)
        (holding ?a - Robot ?b - Box)
        (onground ?a - Box)
        (empty ?a - Robot)
        (clear ?a - Box)
    )
    (:action stack
        :parameters (?R1 - Robot ?B1 - Box ?B2 - Box )
        :precondition (and 
            (clear ?B1)
            (not (holding ?R1 ?B2))
        )
        :effect (and 
            (clear ?B2)
            (on ?B2 ?B1)
            (empty ?R1)
            
        )
    )
    (:action operator
        :parameters (?r1 - Robot ?b2 - Box)
        :precondition (and 
            
            
        )
        :effect (and 
            
            (not (holding ?r1 ?b2))
        )
    )
)
