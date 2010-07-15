## ////////////////////////////////////////////////////////////////////////////
##
## aluelab.r
##
## - get averages from alucomb.r log files
## - stores values, errors and correlation in global variables for further elaboration
##
## ////////////////////////////////////////////////////////////////////////////

library(methods)

source("../../../Common/bin/alu-utils.r")

## ////////////////////////////////////////////////////////////////////////////
## definitions

##-- convert current directory to the corresponding data (or log) directory
aeb.log.dir = file.path("../../../Data", sub("^.*/([^/]*/[^/]*/[^/]*)$", "\\1", getwd()))
aeb.log.dir.base = dirname(aeb.log.dir)

##-- if CL less than minimum, inflate variance with S-factors
aeb.cl.min = pnorm(3, mean=0, sd=1, lower.tail=FALSE)

## ////////////////////////////////////////////////////////////////////////////
## code

##-- global variables for averages and their variance
quant.val = numeric(0)
quant.err = numeric(0)
quant.corr = matrix(0,0,0)
quant.cov = matrix(0,0,0)

##
## add a vector of measurements and their correlation to the list
## to quant.val, quant.err, quant.corr, quant.cov
##
aeb.meas.add = function(add.val, add.err, add.corr=NULL) {
  ##
  ## if a correlation matrix is found, use it to determine which quantities where averaged
  ## and which quantities are non-averaged combined quantities
  ## retain only averaged quantities and their correlation
  ##
  if (!is.null(add.corr)) {
    quant.names.averaged = rownames(add.corr)
  } else {
    quant.names.averaged = names(add.val)
    add.corr = as.matrix(diag.m(rep(1, length(add.val))))
    rownames(add.corr) = names(add.val)
    colnames(add.corr) = names(add.val)
  }

  ##-- assemble correlation matrix
  corr.right = matrix(0, dim(quant.corr)[1], dim(add.corr)[2])
  colnames(corr.right) = colnames(add.corr)
  corr.top = cbind(quant.corr, corr.right)
  corr.left = matrix(0, dim(add.corr)[1], dim(quant.corr)[2])
  rownames(corr.left) = rownames(add.corr)
  corr.bottom = cbind(corr.left, add.corr)
  quant.corr <<- rbind(corr.top, corr.bottom)

  ##-- assemble averaged quantities
  quant.averaged.sel = names(add.val) %in% quant.names.averaged
  quant.val <<- c(quant.val, add.val[quant.averaged.sel])
  quant.err <<- c(quant.err, add.err[quant.averaged.sel])

  ##-- update variance matrix
  quant.cov <<- quant.corr * (quant.err %o% quant.err)
}

##
## add a single additional uncorrelated measurement
## to quant.val, quant.err, quant.corr, quant.cov
##
aeb.meas.add.single = function(label, val, err) {
  names(val) = label
  names(err) = label
  aeb.meas.add(val, err)
}

##
## stores results in alucomb.r log files into global variables
## quant.val, quant.err, quant.corr, quant.cov
##
aeb.collect.data = function(items) {
  for (item in items) {
    if (regexpr("[.]log$", item) != -1) {
      file = item
    } else {
      file = file.path(aeb.log.dir.base, item, "average_alucomb.log")
    }
    if (!file_test("-f", file)) {
      cat("error: cannot find alucomb log file", file, "\n")
      next
    }
    ##-- get alucomb results
    rc = get.alucomb(file)
    if (length(rc) == 0) {
      cat("error: cannot read alucomb log file\n  ", file.path(aeb.log.dir.base, item, "average_alucomb.log"), "\n", sep="")
      ## cat("make -C", dir, " update_alucomb\n")
      ## warning("Cannot read alucomb log file\n  ", file.path(aeb.log.dir.base, item, "average_alucomb.log"))
      next
    }
    
    ##-- default correlation matrix (NULL if not read)
    corr.current = rc$corr
    conf.lev = pchisq(rc$chisq, df=rc$dof, lower.tail=FALSE)
    if (conf.lev < aeb.cl.min) {
      ##-- inflate errors if CL < CL_min
      rc$err = rc$err * rc$sfact
      ##-- correlation after S-factor error inflation
      corr.current = rc$corr2
    }
    
    aeb.meas.add(rc$val, rc$err, corr.current)
  }
}

##
## compute linear combination given measurements and their variance
##
aeb.linear.comb = function(lc, val, cov) {
  lc.val = val %*% lc
  lc.var = drop(lc %*% cov %*% lc)
  lc.err = sign(lc.var)*sqrt(abs(lc.var))
  return(c(val=lc.val, err=lc.err))
}

##
## compute linear combinations using measurements stored
## in quant.val, quant.cov
##
aeb.linear.comb.glob = function(lc) {
  if (length(quant.val) == 0) {
    stop("error: no data were loaded")
  }
  diff = setdiff(names(lc), names(quant.val))
  if (length(diff) > 0) {
    stop("error: following quantities were not loaded: ", diff)
  }
  meas.lc = names(lc)
  return(aeb.linear.comb(lc, quant.val[meas.lc], quant.cov[meas.lc, meas.lc]))
}
