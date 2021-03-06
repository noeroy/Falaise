# -*- mode: conf-unix; -*-
# @falaise:config/snemo/demonstrator/geometry/4.0/models/source_module/basic/source_strips.geom


#################
# Source strips #
#################

# Description of source strips geometry for 'basic' source layout

###############################################################################
[name="source_external_film.basic.model" type="geomtools::simple_shaped_model"]

shape_type         : string  = "box"
x                  : real as length =   12.0 um
y                  : real as length =  125.0 mm
z                  : real as length = 3350.0 mm

material.ref       : string  = "basic::mylar"

visibility.hidden  : boolean = false
visibility.color   : string  = "cyan"


###############################################################################
[name="source_internal_film.basic.model" type="geomtools::simple_shaped_model"]

shape_type         : string  = "box"
x                  : real as length =   12.0 um
y                  : real as length =  135.5 mm
z                  : real as length = 3350.0 mm

material.ref       : string  = "basic::mylar"

visibility.hidden  : boolean = false
visibility.color   : string  = "cyan"


###############################################################################
#
#        ^ z
#        :
#    +-------+
#    |   :   |
#    | +---+ |
#    | | : | |
#    | | : | | pad #7
#    | | : | |
#    | +---+ |
#    | : : : |
# ---|-:-+-:-|----> y
#    | : : : |
#    | +---+ |
#    | | : | |
#    | | : | | pad #1
#    | | : | |
#    | +---+ |
#    | | : | |
#    | | : | | pad #0
#    | | : | |
#    | +---+ |
#    |   :   |
#    +-------+
#        :
#
[name="source_external_multipad.basic.model" type="geomtools::replicated_model"]

#@description The stacking axis
replicated.axis : string = "z"

#@description The number of stacked volumes
replicated.number_of_items : integer = 8

#@description The model of replicated volumes
replicated.model        : string = "source_external_pad.basic.model"

#@description The label of replicated volumes
replicated.label        : string = "pads"

#@description The replicated step
replicated.step         : real as length = 337.5 mm

#@description Height of the mother box
z                       : real as length = 3350.0 mm

material.ref : string = "basic::mylar"

visibility.hidden           : boolean = false
visibility.envelop_hidden   : boolean = true
visibility.daughters.hidden : boolean = false
visibility.color            : string = "grey"

# Mapping the source pads :
mapping.daughter_id.pads : string = "[source_pad:pad+0]"


###############################################################################
#
#        ^ z
#        :
#    +-------+
#    |   :   |
#    | +---+ |
#    | | : | |
#    | | : | | pad #7
#    | | : | |
#    | +---+ |
#    | : : : |
# ---|-:-+-:-|----> y
#    | : : : |
#    | +---+ |
#    | | : | |
#    | | : | | pad #1
#    | | : | |
#    | +---+ |
#    | | : | |
#    | | : | | pad #0
#    | | : | |
#    | +---+ |
#    |   :   |
#    +-------+
#        :
#
[name="source_internal_multipad.basic.model" type="geomtools::replicated_model"]

#@description The stacking axis
replicated.axis : string = "z"

#@description The number of stacked volumes
replicated.number_of_items : integer = 8

#@description The model of replicated volumes
replicated.model        : string = "source_internal_pad.basic.model"

#@description The label of replicated volumes
replicated.label        : string = "pads"

#@description The replicated step
replicated.step         : real as length = 337.5 mm

#@description Height of the mother box
z                       : real as length = 3350.0 mm

material.ref : string = "basic::mylar"

visibility.hidden           : boolean = false
visibility.envelop_hidden   : boolean = true
visibility.daughters.hidden : boolean = false
visibility.color            : string = "grey"

# Mapping the source pads :
mapping.daughter_id.pads : string = "[source_pad:pad+0]"


#########################################################################
#
#               ^ z
#               :
#               :
#           full strip
#           +-+---+-+
#           | | : | |
#           | | : | |
#           | | s | |
#         f | | o | | f
#         i | | u | | i
# - - - - l | | r | | l - - - -> x
#         m | | c | | m
#         0 | | e | | 1
#           | | : | |
#           | | : | |
#           | | : | |
#           +-+---+-+
#               :
#               :
#
[name="source_internal_strip.basic.model" type="geomtools::stacked_model"]

stacked.axis            : string = "x"
stacked.number_of_items : integer = 3

stacked.model_0   : string = "source_internal_film.basic.model"
stacked.label_0   : string = "back_film"

stacked.model_1   : string = "source_internal_multipad.basic.model"
stacked.label_1   : string = "source"

stacked.model_2   : string = "source_internal_film.basic.model"
stacked.label_2   : string = "front_film"

material.ref      : string = "tracking_gas"

visibility.color  : string = "green"

# Mapping the daughter volumes :
mapping.daughter_id.back_film  : string = "[source_strip_film:film=0]"
mapping.daughter_id.front_film : string = "[source_strip_film:film=1]"


#########################################################################
#
#               ^ z
#               :
#               :
#           full strip
#           +-+---+-+
#           | | : | |
#           | | : | |
#           | | s | |
#         f | | o | | f
#         i | | u | | i
# - - - - l | | r | | l - - - -> x
#         m | | c | | m
#         0 | | e | | 1
#           | | : | |
#           | | : | |
#           | | : | |
#           +-+---+-+
#               :
#               :
#
[name="source_external_strip.basic.model" type="geomtools::stacked_model"]

stacked.axis            : string = "x"
stacked.number_of_items : integer = 3

stacked.model_0   : string = "source_external_film.basic.model"
stacked.label_0   : string = "back_film"

stacked.model_1   : string = "source_external_multipad.basic.model"
stacked.label_1   : string = "source"

stacked.model_2   : string = "source_external_film.basic.model"
stacked.label_2   : string = "front_film"

material.ref      : string = "tracking_gas"

visibility.color  : string = "green"

# Mapping the daughter volumes :
mapping.daughter_id.back_film  : string = "[source_strip_film:film=0]"
mapping.daughter_id.front_film : string = "[source_strip_film:film=1]"


# end of @falaise:config/snemo/demonstrator/geometry/4.0/models/source_module/basic/source_strips.geom