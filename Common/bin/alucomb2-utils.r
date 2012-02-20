#!/usr/bin/env Rscript

## ////////////////////////////////////////////////////////////////////////////
## definitions

##
## create diagonal matrix also for vectors of length one
##
diag.m <- function(vec) {
  if (length(vec) <= 1) {
    rc = as.matrix(vec)
  } else {
    rc = diag(vec)
  }
  rc
}

##
## print matrix with even formatting
##
alu.rbind.print = function(x, width=13, num.columns=NULL) {
  number.width = width
  rn.max = max(nchar(rownames(x)))
  cn.max = max(nchar(colnames(x)))
  cn.max = max(number.width, cn.max)
  fmt = paste("% -", number.width, ".", number.width-7, "g", sep="")
  width = getOption("width")
  digits = getOption("digits")
  if (is.null(num.columns)) {
    items.per.row = max(1, floor((width-rn.max) / (cn.max+1))) 
  } else {
    items.per.row = num.columns
  }
  for (i.first in seq(1, ncol(x), by=items.per.row)) {
    ## print(format(x), quote=FALSE)
    i.last = min(i.first + items.per.row - 1, ncol(x))
    cat(format("", width=rn.max+1), paste(format(colnames(x)[i.first:i.last], width=cn.max), collapse=" "), "\n", sep="")
    mapply(function(label, vec) {
      cat(format(label, width=rn.max), " ", paste(format(sprintf(fmt, unlist(vec)), width=cn.max), collapse=" "), "\n", sep="")
    }, rownames(x), apply(x[,i.first:i.last, drop=FALSE], 1, list))
  }
  return(invisible(NULL))
}

##
## test if pattern matches string irrespective of letters case
##
match.nocase = function(pattern, str) {
  return(regexpr(pattern, str, ignore.case=TRUE) != -1)
}

##--- measurement names matching a specific name
meas.match = function(gamma) {
  return( regexpr(paste("[.]", gamma, "[.]", sep=""), meas.names) != -1 )
}

##--- sum in quadrature
quadrature = function(x) {
  return(sqrt(sum(x^2)))
}

##--- normalize publication state
alu.norm.pubstate = function(str) {
  str = sub("^pub[^.]*", "pub", str, perl=TRUE)
  str = sub("^sub[^.]*", "sub", str, perl=TRUE)
  str = sub("^acc[^.]*", "acc", str, perl=TRUE)
  str = sub("^prelim[^.]*", "prelim", str, perl=TRUE)
  str = sub("^PUB[^.]*", "PUB", str, perl=TRUE)
  str = sub("^SUB[^.]*", "SUB", str, perl=TRUE)
  str = sub("^ACC[^.]*", "ACC", str, perl=TRUE)
  str = sub("^PRELIM[^.]*", "PRELIM", str, perl=TRUE)
  return(str)
}

##
## cards
##

##
## measurement block:
##
## old style
##   BEGIN <exp> <quantity> <pub|prelim> <reference>.[<other.tags>]
##   MEASUREMENT    m_<quantity>  statistical  systematic 
##   DATA           m_<quantity>  statistical  systematic 
##                  <val>         <stat>       <syst>
##   ...
##   STAT_CORR_WITH 0.235119 BaBar Gamma9by5 pub AUBERT.10P
##   ...
##   ##--- systematics terms
##   DATA      
##     term1 <signed val>
##     term2 <signed val>
##   END
## new style
##   BEGIN MEASUREMENT <exp> <quantity> <pub|prelim> <reference> [<other tags>]
##   VALUE <val> <stat> <syst>
##   ...
##   SYSTEMATICS
##     term1 <signed val>
##     term2 <signed val>
##   ...
##   END
## 
## a "measurement" is stored in a list with fields
##
## - tags = character array: measurement name fields
## - quant = character: measured quantity
## - value = numeric,
## - stat = numeric,
## - syst = numeric,
## - params = list of numeric arrays: parameter value, pos/neg uncertainty
## - syst.terms = numeric array: syst. contributions related to external parameters
## - corr.terms.stat = numeric array: stat. corr. with other measurements
## - corr.terms.tot = numeric array: error corr. with other measurements
## 

##
## combination block:
##
## old style
##   BEGIN <tag> [<tag 2> ...]
##   COMBINE * * *
##   PARAMETERS !      assumed_value  pos_excurs   neg_excurs
##     par1            0.919          +0.003       -0.003
##   ...
##   MEASUREMENT m_<quantity> statistical systematic [seed <value>] [scale <value>]
##   ...
##   END
## new style
##   BEGIN COMBINATION <tag> [<tag 2> ...]
##   PARAMETERS !      assumed_value  pos_excurs   neg_excurs
##     par1            0.919          +0.003       -0.003
##   ...
##   QUANTITY <quantity> [seed <value>] [scale <value>]
##   ...
##   END
##
## a "combination" is stored as a list with fields:
##
## - tags = character array: combination name fields
## - quantities = character array: measured quantities to be fitted
## - params = list of numeric arrays: parameter value, pos/neg uncertainty
## - constr.lin.comb = list of numeric arrays: coeffs of quantities in linear constraints
## - constr.lin.val = list or numeric: values of linear constraint equations
## - constr.nl.expr = list of numeric arrays: coeffs of quantities in linear constraints
## - constr.nl.val = list or character: expression of non-linear constraint
##

## ////////////////////////////////////////////////////////////////////////////
## code

##
## alucomb.read
##

alucomb.read = function(file = "") {
  if (match.nocase("^\\s*$", file)) {
    stop("alucomb: please provide as argument the card input file\n")
  }
  
  flag.replace.corr = FALSE
  flag.build.delta = FALSE
  
  if (!file.exists(file)) {
    stop("cannot find file ", file, "\n")
  }
  dir.base = dirname(file)
  lines = readLines(file)
  cat("read file", file, "\n")
  
  ##
  ## deal with INCLUDE lines
  ##
  iline = 1
  lines.len = length(lines)
  repeat {
    if (iline > lines.len) break
    fields = unlist(strsplit(lines[iline], "\\s+", perl=TRUE))
    if (length(fields) > 0 &&
        match.nocase("^INCLUDE$", fields[1])) {
      file.inc = paste(dir.base, fields[2], sep="/")
      if (!file.exists(file.inc)) {
        stop("cannot find included file ", file.inc, "\n")
      }
      lines.inc = readLines(file.inc)
      cat("read file", file.inc, "\n")
      lines = c(
        if (iline-1>=1) {lines[1:(iline-1)]} else {NULL},
        lines.inc,
        if (iline+1<=lines.len) {lines[(iline+1):(lines.len)]} else {NULL}
        )
      lines.len = length(lines)
      next
    }
    iline = iline + 1
  }
  
  ##
  ## init global storage for cards
  ##
  measurements = list()
  combination = list()
  
  s.inblock = FALSE
  s.inclause = FALSE
  
  t.inblock = FALSE
  t.inclause = FALSE
  t.continue = FALSE
  t.endclause = FALSE
  t.endblock = FALSE
  t.endfile = FALSE
  
  iline = 1
  lines.len = length(lines)
  repeat {
    ##
    ## read new line
    ##
    if (iline > lines.len) {
      t.endfile = TRUE
    } else {
      line = lines[iline]
      iline = iline+1
      
      ##--- skip comment and empty lines
      if (regexpr("^\\s*$", line, perl=TRUE) != -1 ||
          regexpr("^[\\#!*;]", line, perl=TRUE) != -1) {
        next
      }
      ##--- remove trailing whitespace
      line = sub("\\s+$", "", line, perl=TRUE)
      ##--- remove end-of-line comments
      line = sub("\\s*[\\#!].*$", "", line, perl=TRUE)

      ##
      ## tokenize input
      ##

      ##
      ## split line in quoted strings segments
      ## matched substrings will be
      ## - a string for any unquoted sequence between line-begin, line-end or double-quotes
      ## - a string for every quoted string
      ## - an empty string both before and after any quoted string
      ##
      matches = gregexpr("([^\"\\]*(\\.[^\"\\]*)*)", line)[[1]]
      if (length(matches) > 1) {
        ##--- special treatment for lines with quoted strings
        matches.len = attr(matches, "match.length")
        matches.zerolen = which(matches.len == 0)
        matches.qstr = matches.zerolen[c(TRUE, FALSE)] + 1
        matches.str = setdiff(seq(1, length(matches)), c(matches.zerolen, matches.qstr))
        matches.all.ind = sort(c(matches.str, matches.qstr))
        matches.qstr.flag = matches.all.ind %in% matches.qstr
        matches.all = matches[matches.all.ind]
        attr(matches.all, "match.length") = matches.len[matches.all.ind]
        line.qstrings = regmatches(line, list(matches.all))[[1]]
        ##--- split
        fields = unlist(mapply(function(word, qstr.flag) {
          if (qstr.flag) {return(word)}
          ##--- trim and split unquoted strings
          word = sub("^\\s+|\\s+$", "", word)
          unlist(strsplit(word, "\\s+", perl=TRUE))
	}, line.qstrings, matches.qstr.flag, USE.NAMES=FALSE))
        ##--- restore empty string that strsplit would return for lines that begin with whitespace
        if (regexpr("^\\s+", line, perl=TRUE) != -1) {
          fields = c("", fields)
        }
      } else {
        fields = unlist(strsplit(line, "\\s+", perl=TRUE))
      }
      
      ##--- block begin
      t.inblock = (fields[1] == "BEGIN")
      ##--- block end
      t.endblock = (fields[1] == "END")
      ##--- a line beginning with space is the continuation of the last clause
      t.continue = (fields[1] == "")
      ##--- begin a clause if: 1) does not begin/end a block and 2) does not begin with blank
      t.inclause = !(t.inblock || t.endblock || t.continue)
    }
    
    ##--- when continuing a clause collect all its fields
    if (t.continue) {
      if (s.inclause) {
        ##--- continuation of previous clause
        clause.fields = c(clause.fields, fields[-1])
      } else {
        t.continue = FALSE
        cat("warning, continuation line outside of clause, ignored, line ...\n")
        cat("  '", line, "'\n", sep="")
      }
      next
    }
    
    ##
    ## end of a clause -> interpret its fields
    ##
    if (s.inclause && !t.continue) {
      ##--- if no continuation then the current clause ends
      s.inclause = FALSE
      if (t.inblock) {
        cat("warning, clause ended by BEGIN block, line ...\n")
        cat("  '", line, "'\n", sep="")
      }
      if (t.endfile) {
        cat("warning, clause ended by end of file, clause...\n")
        cat("  ", paste(clause.fields, collapse=" "), "\n", sep="")
      }
      ##
      ## handle a complete clause here
      ##
      ## cat("END clause", unlist(clause.fields), "\n\n")
      clause.keyw = toupper(clause.fields[1])
      clause.fields = clause.fields[-1]
      
      ##
      ## values are numbers possibly preceded by "+", "-", "+-"
      ## sequences "+num1 -num2" are replaced by a +num1 with attribute "negval" = -num2
      ## the word +-num is replaced by +num with attribute "pm"
      ##
      ##--- for each word get any of "
      clause.fields.signtag = sub("^([+]|-|[+]-|).*$", "\\1", clause.fields)
      ##--- remove "+-" sequence at begin of word, which would stop conversion to numeric
      clause.fields.nopm = sub("^[+]-", "", clause.fields)
      clause.fields.val = suppressWarnings(as.numeric(clause.fields.nopm))
      ##--- labels are all non-numeric words
      clause.labels = clause.fields[is.na(clause.fields.val)]
      
      ##--- indices to numeric values preceded by plus sign
      plus.ind = grep("+", clause.fields.signtag, fixed=TRUE)
      plus.ind = plus.ind[!is.na(clause.fields.val[plus.ind])]
      ##--- indices to numeric values preceded by minus sign
      minus.ind = grep("-", clause.fields.signtag, fixed=TRUE)
      minus.ind = minus.ind[!is.na(clause.fields.val[minus.ind])]
      
      ##--- indices to sequences +<numeric value 1> -<numeric value 2>
      plus.minus.ind = intersect(plus.ind, minus.ind-1)
      ##--- add negative value as attribute to positive value
      plus.minus.val = mapply(
        function(first, second) {
          attr(first, "negval", second)
          return(first)
        }, clause.fields.val[plus.minus.ind], clause.fields.val[plus.minus.ind+1])
      ##--- replace +<numeric value 1> -<numeric value 2> with a single number
      clause.fields.val = as.list(clause.fields.val)
      clause.fields.val[plus.minus.ind] = plus.minus.val
      clause.fields.val[plus.minus.ind+1] = NA
      pm.ind = grep("+-", clause.fields.signtag, fixed=TRUE)
      clause.fields.val[pm.ind] = lapply(clause.fields.val[pm.ind], function(val) {attr(val, "pm") = TRUE; val})
      clause.values = clause.fields.val[!is.na(clause.fields.val)]

      if (clause.keyw == "COMBINE") {
        ##
        ## COMBINE
        ##
        
        ##+++ combos
        block.type = "COMBINATION"
        ##+++ combos
        clause.labels = clause.labels[clause.labels != "*"]
        ##--- collect quantities to combine        
        block$combine = c(block$combine, clause.labels)
        
      } else if (clause.keyw == "PARAMETERS") {
        ##
        ## PARAMETERS (same treatment in MEASUREMENT and COMBINATION block)
        ##
        
        ##--- after collapsing +devp -devm into devp with attr negval expect 2x values as labels
        if (length(clause.values) != 2*length(clause.labels)) {
          stop("wrong parameter data in line...\n  ", paste(c(clause.keyw, clause.fields), collapse=" "))
        }
        param.val = clause.values[seq(1, 2*length(clause.labels), by=2)]
        names(param.val) = clause.labels
        param.delta.p = clause.values[seq(2, 2*length(clause.labels), by=2)]
        param.delta.n = lapply(param.delta.p, function(delta) {
          ifelse(is.null(attr(delta, "negval")), -delta, attr(delta, "negval"))
        })
        block$params = c(block$params, mapply(
          function(val, delta.p, delta.n) {
            c("value"=val, "delta_pos"=delta.p, "delta_neg"=delta.n)
          }, param.val, param.delta.p, param.delta.n, SIMPLIFY=FALSE))
        
      } else if (clause.keyw == "QUANTITY" || (block.type == "COMBINATION" && clause.keyw == "MEASUREMENT")) {
        ##
        ## QUANTITY or (+++combos) MEASUREMENT in COMBINE block
        ##
        
        ##--- get first label, quantity name
        ##+++ combos, remove leading "m_"
        meas.name = sub("^m_", "", clause.fields[1], ignore.case=TRUE)
        if (is.null(block$quantities[[meas.name]])) {
          block$quantities[[meas.name]] = list()
        }
        clause.labels = clause.labels[-1]

        ##+++ combos, remove labels named "stat*", "syst*"
        clause.labels = clause.labels[!match.nocase("^(syst|stat)", clause.labels)]
        
        ##
        ## get label/values where values are strings
        ## for QUANTITY it is required that each value follows its label
        ##
        str.labels.mask = clause.labels %in% c("node", "descr")
        if (tail(str.labels.mask, 1)) {
          stop("label without a value in line...\n  ", paste(c(clause.keyw, clause.fields), collapse=" "))
        }
        str.values.mask = c(FALSE, head(str.labels.mask, -1))
        ##--- add at beginning string labels/values before numeric label/values
        clause.values = c(as.list(clause.labels[str.values.mask]), as.list(clause.values))
        clause.labels = c(clause.labels[str.labels.mask], clause.labels[!(str.labels.mask | str.values.mask)])

        if (length(clause.labels) != length(clause.values)) {
          stop("mismatch between labels and numeric values in line...\n  ", paste(c(clause.keyw, clause.fields), collapse=" "))
        }

        if (length(clause.values) > 0) {
          labels.exist = names(block$quantities[[meas.name]])
          labels.override = clause.labels[clause.labels %in% labels.exist]
          if (any(labels.override)) {
            cat("warning, override quantity", meas.name, "\n")
            rc = alu.rbind.print(rbind(
              unlist(block$quantities[[meas.name]][labels.override]),
              clause.values[labels.override]
              ))
          }
          block$quantities[[meas.name]][clause.labels] = clause.values
        }
        
      } else if (clause.keyw == "MEASUREMENT") {
        ##
        ##+++ MEASUREMENT clause, combos compatibility
        ##
        block.type = "MEASUREMENT"
        meas.name = sub("^m_", "", clause.fields[1], ignore.case=TRUE)
        if (meas.name != block.fields[2]) {
          stop("MEASUREMENT quantity does not match BLOCK quantity...\n  ", paste(c(clause.keyw, clause.fields), collapse=" "))
        }
        measurement.in.data = TRUE
      } else if (clause.keyw == "VALUE" || (clause.keyw == "DATA" && measurement.in.data)) {
        ##
        ## VALUE --> enter measurement value
        ##+++ DATA after MEASUREMENT clause, combos compatibility
        ##
        
        if (length(clause.values) == 3) {
          if (!is.null(attr(clause.values[[1]], "negval"))) {
            stop("\"+-\" used for measurements value in line...\n  ", paste(c(clause.keyw, clause.fields), collapse=" "))
          }
          ##--- use values if existing
          block.meas$value = clause.values[[1]]
          
          block.meas$stat.p = clause.values[[2]]
          negval = attr(clause.values[[2]], "negval")
          if (!is.null(negval)) {
            block.meas$stat.n = negval
            if (block.meas$stat.n == -block.meas$stat.p) {
              block.meas$stat = block.meas$stat.p
            } else {
              block.meas$stat = sqrt((block.meas$stat.p^2 + block.meas$stat.n^2)/2)
            }
          } else {
            block.meas$stat.n = -block.meas$stat.p
            block.meas$stat = block.meas$stat.p
          }
          
          block.meas$syst.p = clause.values[[3]]
          negval = attr(clause.values[[3]], "negval")
          if (!is.null(negval)) {
            block.meas$syst.n = negval
            block.meas$syst = sqrt((block.meas$syst.p^2 + block.meas$syst.n^2)/2)
          } else {
            block.meas$syst.n = -block.meas$syst.p
            block.meas$syst = block.meas$syst.p
          }

          if (!is.null(measurements[[meas.tag]]$value)) {
            old.meas = measurements[[meas.tag]]
            cat("warning, update measurement", meas.tag, "\n")
            cat("  old VALUE ", old.meas$value,
                " (+", old.meas$stat.p, " ", old.meas$stat.n, ")",
                " (+", old.meas$syst.p, " ", old.meas$syst.n, ")\n",
                sep="")
            cat("  new VALUE ", block.meas$value,
                " (+", block.meas$stat.p, " ", block.meas$stat.n, ")",
                " (+", block.meas$syst.p, " ", block.meas$syst.n, ")\n",
                sep="")
          }
          
          ##+++ set end of MEASEREMENT DATA (combos compatibility)
          measurement.in.data = FALSE
        } else {
          stop("wrong number of numeric data in line...\n  ", paste(c(clause.keyw, clause.fields), collapse=" "))
        }
      } else if (clause.keyw == "DATA" ||
                 clause.keyw == "SYSTEMATICS" || clause.keyw == "SYSTLOCAL" || clause.keyw == "SYSTPAPER" ) {
        ##
        ## SYSTEMATICS or (+++combos) DATA
        ## list systematic terms with a label that possibly refers
        ## to a global parameter or to the same effect in another
        ## measurement
        ##
        ## SYSTLOCAL like SYSTEMATICS but add measurement name to make it local
        ## SYSTPAPER like SYSTEMATICS but add reference name to make it paper-wide
        ##
        
        ##
        ## - "%" on either name or value means percent of measurement value
        ## - "&" on either name or value means relative of measurement value
        ##
        clause.df = data.frame(
          field = clause.fields,
          rel = grepl("^(.*)[&]$", clause.fields, perl=TRUE),
          perc = grepl("^(.*)[%]$", clause.fields, perl=TRUE),
          num = suppressWarnings(as.numeric(clause.fields)),
          stringsAsFactors=FALSE)
        clause.labels = sub("^(.*)[&%]$", "\\1", with(clause.df, field[is.na(num)]))
        clause.values = with(clause.df, num[!is.na(num)])
        if (length(clause.values) != length(clause.labels)) {
          stop("mismatch between labels and numeric values in line...\n  ", paste(c(clause.keyw, clause.fields), collapse=" "))
        }
        clause.perc = with(clause.df, perc[is.na(num)]) | with(clause.df, perc[!is.na(num)])
        clause.rel = with(clause.df, rel[is.na(num)]) | with(clause.df, rel[!is.na(num)])
        clause.values =
          ifelse(clause.perc, clause.values*block.meas$value/100,
                 ifelse(clause.rel, clause.values*block.meas$value,
                        clause.values))
        
        if (!is.null(block.meas$syst.terms)) {
          stop("duplicated systematic terms in line...\n  ", paste(c(clause.keyw, clause.fields), collapse=" "))
        }
        if (clause.keyw == "SYSTLOCAL") {
          clause.labels = paste(paste(block.fields, collapse="."), clause.labels, sep=".")
        } else if (clause.keyw == "SYSTPAPER") {
          clause.labels = paste(paste(block.fields[-2], collapse="."), clause.labels, sep=".")
        }
        names(clause.values) = clause.labels
        block.meas$syst.terms = unlist(clause.values)
        
      } else if (clause.keyw == "STAT_CORR_WITH" || clause.keyw == "ERROR_CORR_WITH") {
        ##
        ## STAT_CORR_WITH, ERROR_CORR_WITH
        ##
        
        if (length(clause.values) != 1) {
          stop("exactly one numeric value required in line...\n  ", paste(c(clause.keyw, clause.fields), collapse=" "))
        }
        corr = clause.values[[1]]
        
        clause.labels[3] = alu.norm.pubstate(clause.labels[3])
        names(corr) =  paste(clause.labels, collapse=".")
        if (clause.keyw == "STAT_CORR_WITH") {
          block.meas$corr.terms.stat = c(block.meas$corr.terms.stat, corr)
        } else {
          block.meas$corr.terms.tot = c(block.meas$corr.terms.tot, corr)
        }

      } else if (clause.keyw == "SUMOFQUANT" || clause.keyw == "COMBOFQUANT" || clause.keyw == "CONSTRAINT") {
        ##
        ## SUMOFQUANT, COMBOFQUANT, CONSTRAINT
        ##
        
        if (clause.keyw != "CONSTRAINT") {
          constr.name = paste(clause.fields[1], "coq", sep=".")
          constr.val = 0
        }
        
        if (clause.keyw == "SUMOFQUANT") {
          constr.comb = c(-1, rep(1, length(clause.fields)-1))
          names(constr.comb) = clause.fields
        } else if (clause.keyw == "COMBOFQUANT") {
          if (length(clause.labels)-1 != length(clause.values)) {
            stop("error: mismatch between labels and data values in line...\n  ",
                 paste(c(clause.keyw, clause.fields), collapse=" "))
          }
          constr.comb = c(-1, unlist(clause.values))
          names(constr.comb) = clause.labels
        } else if (clause.keyw == "CONSTRAINT") {
          if (length(clause.labels) != length(clause.values)) {
            stop("error: mismatch between labels and data values in line...\n  ",
                 paste(c(clause.keyw, clause.fields), collapse=" "))
          }
          constr.name = clause.labels[1]
          constr.val = clause.values[[1]]
          constr.comb = unlist(clause.values[-1])
          names(constr.comb) = clause.labels[-1]
        }
        
        block$constr.lin.val[[constr.name]] = constr.val
        block$constr.lin.comb[[constr.name]] = constr.comb
        
      } else if (clause.keyw == "NLCONSTRAINT") {
        ##
        ## NLCONSTRAINT
        ##
        
        if (is.na(suppressWarnings(as.numeric(clause.fields[2])))) {
          stop("error: missing numeric value in non-linear constraint in...\n  ",
               paste(c(clause.keyw, clause.fields), collapse=" "))
        }
        if (length(clause.labels) != 2) {
          stop("error: NLCONSTRAINT needs one numeric value and one expression in line...\n  ",
               paste(c(clause.keyw, clause.fields), collapse=" "))
        }
        constr.name = clause.labels[1]
        constr.val = clause.values[[1]]
        constr.expr = clause.labels[2]
        block$constr.nl.val[[constr.name]] = constr.val
        block$constr.nl.expr[[constr.name]] = constr.expr
        
      }
    }
    
    if (t.inclause) {
      ##--- new clause (previous one has been handled already)
      t.inclause = FALSE
      if (s.inblock) {
        s.inclause = TRUE
        clause.fields = fields
      } else {
        cat("warning, begin clause outside of block, ignored, line ...\n")
        cat("  '", line, "'\n", sep="")
      }
    }
    
    if (t.inblock) {
      t.inblock = FALSE
      if (s.inblock) {
        cat("error, nested BEGIN block, ignored, line ...\n")
        cat("  '", line, "'\n", sep="")
        next
      }
      s.inblock = TRUE
      
      ##--- remove "BEGIN"
      block.fields = fields[-1]
      
      block.type = ""
      if (length(block.fields) > 0) {
        block.type = toupper(block.fields[1])
        if (block.type == "COMBINATION" || block.type == "MEASUREMENT") {
          block.fields = block.fields[-1]
        } else {
          ##+++ temporarily just warning for COMBOS-like cards compatibility
          if (TRUE) {
            cat("warning, unknown BEGIN block in line...\n")
            cat(" ", fields, "\n")
          }
        }
      }
      ##
      ## initialization both for MEASUREMENT and COMBINATION
      ## when combos compatibility is abandoned, one can init just the relevant list
      ## block$params is used for parameters both of MEASUREMENT and COMBINATION
      ##

      ##--- special treatment in case this is a MEASUREMENT block
      meas.fields = block.fields
      ##+++combos: separate tags joint with dots
      if (length(meas.fields) == 3) {
        meas.fields = c(meas.fields[-3], strsplit(meas.fields[3], "[.]", perl=TRUE)[[1]])
      }
      meas.fields[3] = alu.norm.pubstate(meas.fields[3])
      meas.tag = paste(meas.fields, collapse=".")

      block.meas = list()
      
      block = list()
      block$params = list()
      block$quantities = list()
      block$constr.lin.val = list()
      block$constr.lin.comb = list()
      block$constr.nl.val = list()
      block$constr.nl.expr = list()
    }
    
    if (s.inblock && (t.endblock || t.endfile)) {
      ##--- end block
      t.endblock = FALSE
      s.inblock = FALSE
      ## cat("END block", unlist(block.fields), "\n\n")
      if (block.type == "MEASUREMENT") {
        ##
        ## MEASUREMENT BLOCK
        ##
        
        if (length(block.fields) < 3) {
          stop("too few fields in line...\n  ", paste(c("BEGIN", block.type, block.fields), collapse=" "))
        }

        block.meas$tags = meas.fields
        
        if (!is.null(measurements[[meas.tag]])) {
          cat("warning, updated measurement", meas.tag, "\n")
        }
        block.meas$params = block$params
        ##
        ## measured quantity = 2nd tag in the BEGIN MEASUREMENT statement
        ## BEGIN [MEASUREMENT] <esperiment> <quantity> <pub|prelim> <reference> [...]
        ##
        block.meas$quant = block.fields[2]
        measurements[[meas.tag]] = block.meas
        
      } else if (block.type == "COMBINATION") {
        ##
        ## COMBINE BLOCK
        ##
        block$tags = unlist(block.fields)
        if (is.null(block$combine) || length(block$combine) == 0) {
          ##--- average all quantities mentioned in block if no explicit selection
          block$combine = names(block$quantities)
        }

        ##--- add "USE = 1" quantities, drop "USE = 0" quantities
        quant.use = unlist(lapply(block$quantities, function(el) { unname(el["use"]) }))
        quant.drop = names(quant.use[quant.use == 0])
        quant.use = names(quant.use[quant.use != 0])
        block$combine = setdiff(block$combine, quant.drop)
        block$combine = c(block$combine, setdiff(quant.use, block$combine))

        ##--- insure the descr field has at least an empty string
        quant.descr = names(unlist(lapply(block$quantities, function(el) { unname(el["descr"]) })))
        quant.descr.wo = setdiff(names(block$quantities), quant.descr)
        block$quantities = lapply(block$quantities,
          function(quant) {
            if (is.null(quant$descr)) quant$descr = ""
            quant
          })
        
        combination = block
        
      } else {
        stop("unknown block type in line...\n  ", paste(c("BEGIN", block.type, block.fields[-1]), collapse=" "))
      }
    }
    
    if (t.endblock && !s.inblock) {
      t.endblock = FALSE
      cat("error, END block without BEGIN, ignored, line ...\n")
      cat("  '", line, "'\n", sep="")
    }
    
    if (t.endfile) {
      t.endfile = FALSE
      break
    }
  }
  
  invisible(list(combination=combination, measurements=measurements))
} ##--- end of alucomb.read

##
## transform the name of a measurement in the HFAG-tau format into a format usable by Root
##
hfag.to.root = function(str) {
  if (str == "TauMass") {
    return("m_{#tau} [MeV]")
  }
  
  str.orig = str
  
  str = gsub("Numbar", "#bar{#nu}_{#mu}",str)  
  
  str = gsub("(^|[^A-Z])([A-Z][a-y]*)([z+-]*)b", "\\1#bar{\\2}\\3", str, perl=TRUE)
  str = gsub("F1", "f_{1}",str)
  str = gsub("Pi", "#pi",str)
  
  str = gsub("Nue", "#nu_{e}",str)
  str = gsub("Num", "#nu_{#mu}",str)
  str = gsub("Nut", "#nu_{#tau}",str)
  str = gsub("Nu", "#nu",str)
  
  str = gsub("M", "#mu",str)
  str = gsub("H", "h",str)
  str = gsub("m($|[#}A-Zh])", "^{-}\\1", str, perl=TRUE)
  str = gsub("p($|[#}A-Zh])", "^{+}\\1", str, perl=TRUE)
  str = gsub("z($|[#}A-Zh])", "^{0}\\1", str, perl=TRUE)

  if (str.orig %in% c("HmHmHpNu", "PimKmPipNu", "PimPimPipNu")) {
    str = paste(str, "(ex.K^{0})")
  }

  str = paste("B(#tau^{-} #rightarrow ", str, ")", sep="")
  return(str)
}

##
## compute root of symmetric real positive semi-definite matrix
##
alu.matr.sqrt.symm.semipos = function(X, tol = sqrt(.Machine$double.eps)) {
  if (!is.matrix(X)) 
    X <- as.matrix(X)
  if (length(dim(X)) > 2L || !(is.numeric(X))) 
    stop("'X' must be a numeric or complex matrix")
  
  X = (X + t(X))/2
  rc.e = eigen(X)
  comp.positive = rc.e$values > tol*rc.e$values[1]
  rc.vals = ifelse(comp.positive, rc.e$values, 0)
  rc = rc.e$vectors %*% diag.m(sqrt(rc.vals)) %*% t(rc.e$vectors)
  rc = (rc + t(rc))/2
  rownames(rc) = rownames(X)
  colnames(rc) = colnames(X)
  
  return(rc)
}

##
## compute pseudo-inverse square root of symmetric real positive semi-definite matrix
## zero eigenvalues produce zero eigenvalues in the inverse-square matrix
##
alu.matr.inv.sqrt.symm.semipos = function(X, tol = sqrt(.Machine$double.eps)) {
  if (!is.matrix(X)) 
    X <- as.matrix(X)
  if (length(dim(X)) > 2L || !(is.numeric(X))) 
    stop("'X' must be a numeric or complex matrix")
  
  X = (X + t(X))/2
  rc.e = eigen(X)
  ##--- force to zero computationally zero or negative eigenvalues
  comp.positive = rc.e$values > tol*rc.e$values[1]
  rc.vals = ifelse(comp.positive, rc.e$values, 0)
  rc.vecs.inv = ifelse(rc.vals > 0, 1/rc.vals, 0)
  
  rc.inv = rc.e$vectors %*% diag.m(rc.vecs.inv) %*% t(rc.e$vectors)
  rc.inv = (rc.inv + t(rc.inv))/2
  rownames(rc.inv) = rownames(X)
  colnames(rc.inv) = colnames(X)
  rc = rc.e$vectors %*% diag.m(sqrt(rc.vecs.inv)) %*% t(rc.e$vectors)
  rc = (rc + t(rc))/2
  rownames(rc) = rownames(X)
  colnames(rc) = colnames(X)
  
  attr(rc, "pos.eigen.num") = sum(comp.positive)
  attr(rc, "inv") = rc.inv
  
  attr(rc, "dof.eff") = apply(rc.e$vectors, 1, function(x) sum(x^2 * (rc.vals != 0)))
  ## attr(rc, "dof.factors") = ifelse(attr(rc, "dof.factors") != 0, 1/sqrt(attr(rc, "dof.factors")), 0)
  names(attr(rc, "dof.eff")) = rownames(X)
  
  return(rc)
}

##
## compute pseudo-inverse square root of symmetric real positive semi-definite matrix
## zero eigenvalues produce zero eigenvalues in the inverse-square matrix
## apply provided weight vector to matrix before inversion in order to reduce computational errors
## the resulting matrix Y satisfies: t(Y) %*% Y = X^-1
##
alu.matr.inv.sqrt.symm.semipos.norm = function(X, X.norm, tol = sqrt(.Machine$double.eps)) {
  if (!is.matrix(X)) 
    X <- as.matrix(X)
  if (length(dim(X)) > 2L || !is.numeric(X))
    stop("'X' must be a numeric matrix")
  
  if (dim(X)[1] != dim(X)[2])
    stop("'X' must be a square matrix")
  
  if (!is.numeric(X.norm))
    stop("'X.norm' must be a numeric vector/matrix")
  
  if (length(X.norm) != dim(X)[1])
    stop("X.norm vector dimension must match X matrix rank")
  
  XX = diag.m(1/X.norm) %*% X %*% diag.m(1/X.norm)
  rownames(XX) = rownames(X)
  colnames(XX) = colnames(X)
  XX.inv.sqrt.attr = alu.matr.inv.sqrt.symm.semipos(XX, tol)
  XX.inv.sqrt = XX.inv.sqrt.attr %*% diag.m(1/X.norm)
  rownames(XX.inv.sqrt) = rownames(X)
  colnames(XX.inv.sqrt) = colnames(X)
  
  attr(XX.inv.sqrt, "inv") = diag.m(1/X.norm) %*% attr(XX.inv.sqrt.attr, "inv") %*% diag.m(1/X.norm)
  attr(XX.inv.sqrt, "inv") = (attr(XX.inv.sqrt, "inv")+ t(attr(XX.inv.sqrt, "inv")))/2
  rownames(attr(XX.inv.sqrt, "inv")) = rownames(X)
  colnames(attr(XX.inv.sqrt, "inv")) = colnames(X)
  
  attr(XX.inv.sqrt, "pos.eigen.num") = attr(XX.inv.sqrt.attr, "pos.eigen.num")
  attr(XX.inv.sqrt, "dof.eff") = attr(XX.inv.sqrt.attr, "dof.eff")
  
  return(XX.inv.sqrt)
}