###############################################################################
# nettcl3d.tcl --
#
#    Basic nettcl3d operations
#
###############################################################################

package provide nettcl3d 1.0.0

package require math::statistics 0.5
package require cmdline

namespace eval nettcl3d {
}

# Checks whether program version is greater or equal to given one
# If version checking fails, error is thrown
# Arguments
#   verRequired - version expected, parts are separated by dots, e.g. 1.2.3
proc nettcl3d::matchVersion { verRequired } {
  # ensure nettcl3d commands are available
  if { [info command ::nettcl3d::version] == "" } {
    error "nettcl3d commands are not available"
  }

  # retrieve current vdiff1d version
  set ver [ nettcl3d::version as-list]

  # split given version by parts
  set expected [ split "$verRequired" . ]
  for { set i 0 } { $i < [ llength $expected ] } { incr i } {
    set n [ lindex $expected $i ]
    if { $n > 0 && ( [ llength $ver ] < $i || [ lindex $ver $i ] < $n ) } {
      error "Version mismatch: current version is [nettcl3d::version] but $verRequired expected"
    }
    if { [ lindex $ver $i ] > $n } {
      break
    }
  }
}

# Creates and returns network instance
proc nettcl3d::makeGrid3d { args } {
	set opts {
		{xSize.arg	10		"number of vertices in X dimension"}
		{ySize.arg	10		"number of vertices in Y dimension"}
		{zSize.arg	10		"number of vertices in Z dimension"}
		{betaAvg.arg	""		"Beta average value"}
		{tauAvg.arg		""		"Tau average value"}
		{vAvg.arg		""		"V average value"}
		{delta.arg		""		"Parameter scattering"}
		{seed.arg		12345	"Random generator seed"}
	}

	set usage ": make3dGrid \[options]\noptions:"
	array set options [::cmdline::getoptions args $opts $usage]

	set seedGen [nettcl3d::rng create uniform 10000.0 20000.0]
	nettcl3d::rng seed $seedGen [expr int($options(seed))]
	
	set betaRng [nettcl3d::rng create uniform $options(betaAvg) [expr $options(delta) * $options(betaAvg)]]
	nettcl3d::rng seed $betaRng [expr int([nettcl3d::rng next $seedGen])]
	set tauRng [nettcl3d::rng create uniform $options(tauAvg) [expr $options(delta) * $options(tauAvg)]]
	nettcl3d::rng seed $tauRng [expr int([nettcl3d::rng next $seedGen])]
	set vRng [nettcl3d::rng create uniform $options(vAvg) [expr $options(delta) * $options(vAvg)]]
	nettcl3d::rng seed $vRng [expr int([nettcl3d::rng next $seedGen])]
	
	set populator [::nettcl3d::populator create grid3d	\
		$options(xSize) $options(ySize) $options(zSize) \
		$betaRng $tauRng $vRng	\
	]
	return [::nettcl3d::network create $populator]
}

# Creates and returns tracer instance
proc nettcl3d::makeTracer { args } {
	set opts {
		{interval.arg	0.0		"Tracing interval"}
		{startTime.arg	0.0		"Start time for tracing"}
		{precision.arg	6		"Number precision"}
		{fileName.arg	""		"Trace file name"}
		{tagExpr.arg	""		"Tag expression"}
		{indices.arg	""		"Element indices"}
	}

	set usage ": makeTracer \[options] type\noptions:"
	array set options [::cmdline::getoptions args $opts $usage]
	lassign $args type
	
	switch -exact -- $type {
	    null {
	        return [nettcl3d::tracer create null]
        }
       
	    avg-voltage {
	        return [nettcl3d::tracer create avg-voltage \
	            $options(fileName)  \
	            $options(interval)  \
	            $options(startTime)  \
	            $options(precision)  \
	            $options(tagExpr)  \
            ]
        }
       
        voltage {
	        return [nettcl3d::tracer create voltage \
	            $options(fileName)  \
	            $options(interval)  \
	            $options(startTime)  \
	            $options(precision)  \
	            $options(indices)  \
	            $options(tagExpr)  \
            ]
        }
       
	    avg-flux {
	        return [nettcl3d::tracer create avg-flux \
	            $options(fileName)  \
	            $options(interval)  \
	            $options(startTime)  \
	            $options(precision)  \
	            $options(tagExpr)  \
            ]
        }
        
        flux {
	        return [nettcl3d::tracer create flux \
	            $options(fileName)  \
	            $options(interval)  \
	            $options(startTime)  \
	            $options(precision)  \
	            $options(indices)  \
            ]
        }
        
        default {
            error "Invalid tracer type $type"
        }
    }
}

# Iterates over contacts
proc nettcl3d::foreachContact { varName network tagExpr expression } {
	upvar $varName var
	foreach var [nettcl3d::network get $network contacts $tagExpr] {
		uplevel $expression
	}
}

# Iterates over circuits
proc nettcl3d::foreachCircuit { varName network tagExpr expression } {
	upvar $varName var
	foreach var [nettcl3d::network get $network circuits $tagExpr] {
		uplevel $expression
	}
}

proc nettcl3d::contactIndices { network tagExpr } {
}

