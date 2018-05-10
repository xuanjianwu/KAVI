; logistics domain Typed version.

(define (domain logistics )
    (:requirements :strips :typing :negative-preconditions)
    (:types
        physobj place city - object
        vehicle package - physobj
        location airport - place
        airplane truck - vehicle
    )
    (:predicates
        (in ?a - package ?b - vehicle)
        (in-city ?a - place ?b - city)
        (at ?a - physobj ?b - place)
    )
    (:action LOAD-TRUCK
        :parameters (?pkg - package ?truck - truck ?loc - place )
        :precondition (and 
            (at ?pkg ?loc)
            (at ?truck ?loc)
            
        )
        :effect (and 
            (in ?pkg ?truck)
            (not (at ?pkg ?loc))
        )
    )
    (:action LOAD-AIRPLANE
        :parameters (?pkg - package ?airplane - airplane ?loc - place )
        :precondition (and 
            (at ?pkg ?loc)
            (at ?airplane ?loc)
            
        )
        :effect (and 
            (in ?pkg ?airplane)
            (not (at ?pkg ?loc))
        )
    )
    (:action UNLOAD-TRUCK
        :parameters (?pkg - package ?truck - truck ?loc - place )
        :precondition (and 
            (at ?truck ?loc)
            (in ?pkg ?truck)
            
        )
        :effect (and 
            (at ?pkg ?loc)
            (not (in ?pkg ?truck))
        )
    )
    (:action UNLOAD-AIRPLANE
        :parameters (?pkg - package ?airplane - airplane ?loc - place )
        :precondition (and 
            (at ?airplane ?loc)
            (in ?pkg ?airplane)
            
        )
        :effect (and 
            (at ?pkg ?loc)
            (not (in ?pkg ?airplane))
        )
    )
    (:action DRIVE-TRUCK
        :parameters (?truck - truck ?loc-from - place ?loc-to - place ?city - city )
        :precondition (and 
            (in-city ?loc-from ?city)
            (in-city ?loc-to ?city)
            (at ?truck ?loc-from)
            
        )
        :effect (and 
            (at ?truck ?loc-to)
            (not (at ?truck ?loc-from))
        )
    )
    (:action FLY-AIRPLANE
        :parameters (?airplane - airplane ?loc-from - airport ?loc-to - airport )
        :precondition (and 
            (at ?airplane ?loc-from)
            
        )
        :effect (and 
            (at ?airplane ?loc-to)
            (not (at ?airplane ?loc-from))
        )
    )
)
