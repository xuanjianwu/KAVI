(define (problem logistics-4-0)
    (:domain logistics )
    (:objects
         obj23 obj22 obj21 obj13 obj12 obj11 - package
         tru2 tru1 - truck
         apn1 - airplane
         cit2 cit1 - city
         pos2 pos1 - location
         apt2 apt1 - airport
    )
    (:init
        (at obj11 pos1)
        (at obj12 pos1)
        (at obj13 pos1)
        (at obj21 pos2)
        (at obj22 pos2)
        (at tru1 pos1)
        (at tru2 pos2)
        (at obj23 pos2)
        (at apn1 apt2)
        (in-city pos1 cit1)
        (in-city pos2 cit2)
        (in-city apt2 cit2)
        (in-city apt1 cit1)
    )
    (:goal
        (and 
            (at obj13 apt1)
            (at obj21 pos1)
            (at obj11 apt1)
            (at obj23 pos1)
        )
    )
)