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
        (at ?a - physobj ?b - place)
        (in-city ?a - place ?b - city)
        (in ?a - package ?b - vehicle)
    )
    (:action LOAD-TRUCK
        :parameters (?pkg - package ?truck - truck ?loc - place )
        :precondition (and 
            (at ?truck ?loc)
            (at ?pkg ?loc)
            
        )
        :effect (and 
            (in ?pkg ?truck)
            (not (at ?pkg ?loc))
        )
    )
    (:action LOAD-AIRPLANE
        :parameters (?pkg - package ?airplane - airplane ?loc - place )
        :precondition (and 
            (at ?airplane ?loc)
            (at ?pkg ?loc)
            
        )
        :effect (and 
            (in ?pkg ?airplane)
            (not (at ?pkg ?loc))
        )
    )
    (:action UNLOAD-TRUCK
        :parameters (?pkg - package ?truck - truck ?loc - place )
        :precondition (and 
            (in ?pkg ?truck)
            (at ?truck ?loc)
            
        )
        :effect (and 
            (at ?pkg ?loc)
            (not (in ?pkg ?truck))
        )
    )
    (:action UNLOAD-AIRPLANE
        :parameters (?pkg - package ?airplane - airplane ?loc - place )
        :precondition (and 
            (in ?pkg ?airplane)
            (at ?airplane ?loc)
            
        )
        :effect (and 
            (at ?pkg ?loc)
            (not (in ?pkg ?airplane))
        )
    )
    (:action DRIVE-TRUCK
        :parameters (?truck - truck ?loc-from - place ?loc-to - place ?city - city )
        :precondition (and 
            (in-city ?loc-to ?city)
            (in-city ?loc-from ?city)
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
