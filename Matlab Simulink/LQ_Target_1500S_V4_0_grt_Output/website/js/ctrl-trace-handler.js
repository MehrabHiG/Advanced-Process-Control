moduleSWV.controller("ctrl-trace-handler",function ($scope, $log, $http,$interval) {
    $log.log("[ctrl-trace-handler]: wurde initialisiert");

    $scope.Trace_Config = null;
    $scope.TPS_Config = 0;
    $scope.plot = null;
    $scope.defaultTotalPoints = 10;
    $scope.defaultUpdateInterval = NaN;
    $scope.dataset = null;
    $scope.IntervalProcess = null;
    $scope.startTime;
    $scope.defaultMaxY = 100;
    $scope.defaultMinY = 0;
    $scope.traceParams = [];
	
    $log.log("[ctrl-trace-handler]: lade TPS config");
    //todo bugfix for Model testing
    $http.get("param/Model_TestPointedSignals.json",{headers:{'If-Modified-Since': '0',
    "Pragma": "no-cache",
    "Expires": -1,
    "Cache-Control": "no-cache, no-store, must-revalidate"}}).then(function (response) {
        $scope.TPS_Config = response.data;
    });
	
    $scope.onChangeTotalPoints = function () {
        var lan = document.getElementById("Main").className;
        var num = parseFloat(document.getElementsByName(lan + "TotalPoints")[0].value);
        if (!isNaN(num)) {
            $scope.setTraceParam('totalPoints', num);
        }
    }

    $scope.onChangeMinY = function () {
        var lan = document.getElementById("Main").className;
        var num = parseFloat(document.getElementsByName(lan + "MinY")[0].value);
        if (!isNaN(num)) {
            $scope.setTraceParam('minY', num);
        }
    }

    $scope.onChangeMaxY = function () {
        var lan = document.getElementById("Main").className;
        var num = parseFloat(document.getElementsByName(lan + "MaxY")[0].value);
        if (!isNaN(num)) {
            $scope.setTraceParam('maxY', num);
        }
    }

    $scope.onChangeUpdateInterval = function () {
        var lan = document.getElementById("Main").className;
        var num = parseFloat(document.getElementsByName(lan + "UpdateInterval")[0].value);
        if (!isNaN(num)) {
            $scope.setTraceParam('updateInterval', num);
            if ($scope.IntervalProcess !== null) {
                $interval.cancel($scope.IntervalProcess);
                $scope.IntervalProcess = $interval(function () {
                    if ($scope.Trace_Config !== null) {
                        $scope.PlotRealtimeScope($scope.Trace_Config);
                    }
                }, $scope.getTraceParam('updateInterval'));
            }
        }
    };
	
	$scope.getTraceParam = function(param) {
		if($scope.ShowTrace === false || $scope.CurrentTrace === "") {
			return -1;
		}
		for (var i = 0; i < $scope.traceParams.length; i++) {
			if(($scope.traceParams[i].name === $scope.CurrentTrace)&&($scope.traceParams[i].fullblockpath === $scope.CurrentSvgModel.FullBlockPath)){
				return $scope.traceParams[i][param];
			}
		}
		$scope.setDefaultParamsForCurrentTrace();
		return $scope.getTraceParam(param);
	};
	
	$scope.setTraceParam = function(param,val) {
		if($scope.ShowTrace === false || $scope.CurrentTrace === "") {
			return;
		}
		for (var i = 0; i < $scope.traceParams.length; i++) {
			if(($scope.traceParams[i].name === $scope.CurrentTrace)&&($scope.traceParams[i].fullblockpath === $scope.CurrentSvgModel.FullBlockPath)){
				$scope.traceParams[i][param] = val;
				return;
			}
		}
		$scope.setDefaultParamsForCurrentTrace();
		$scope.setTraceParam(param,val);
	};
	
	$scope.setParamsForCurrentTrace = function(modelname) {
		document.getElementsByName("enTotalPoints")[0].value = $scope.getTraceParam('totalPoints');
		document.getElementsByName("deTotalPoints")[0].value = $scope.getTraceParam('totalPoints');
		document.getElementsByName("enUpdateInterval")[0].value = $scope.getTraceParam('updateInterval');
		document.getElementsByName("deUpdateInterval")[0].value = $scope.getTraceParam('updateInterval');
		document.getElementsByName("enMinY")[0].value = $scope.getTraceParam('minY');
		document.getElementsByName("deMinY")[0].value = $scope.getTraceParam('minY');
		document.getElementsByName("enMaxY")[0].value = $scope.getTraceParam('maxY');
		document.getElementsByName("deMaxY")[0].value = $scope.getTraceParam('maxY');
	}
	
	$scope.setDefaultParamsForCurrentTrace = function() {
		if($scope.ShowTrace === true || $scope.CurrentTrace !== "") {
			$scope.traceParams.push({ name: $scope.CurrentTrace, fullblockpath: $scope.CurrentSvgModel.FullBlockPath, totalPoints: $scope.defaultTotalPoints, updateInterval: $scope.defaultUpdateInterval, minY: $scope.defaultMinY, maxY: $scope.defaultMaxY })
		}
	}
	
    $scope.startTrace = function () {
        $log.log("[ctrl-trace-handler]: start trace");
        $http.get("param/"+ $scope.CurrentTrace +".json",{headers:{'If-Modified-Since': '0',
    "Pragma": "no-cache",
    "Expires": -1,
    "Cache-Control": "no-cache, no-store, must-revalidate"}}).then(function (response) {
            $scope.Trace_Config = response.data;
        });

        $scope.data_Trace = [];
        $scope.startTime = $scope.TPS_Values.Time;
        $scope.IntervalProcess = $interval(function () {
            if($scope.Trace_Config !== null){
                $scope.PlotRealtimeScope($scope.Trace_Config);
            }
		}, $scope.getTraceParam('updateInterval'));
		var lan = document.getElementById("Main").className;
		document.getElementsByName(lan+"StopTrace")[0].removeAttribute("disabled");
		document.getElementsByName(lan+"StartTrace")[0].setAttribute("disabled", "true");
    };

    $scope.closeTrace = function () {
        if($scope.plot === null){
            $log.log("[ctrl-trace-handler]: close trace");
            $scope.$emit("close-Trace");
        }
        else {
            $scope.stopTrace();
            $log.log("[ctrl-trace-handler]: close trace");
            $scope.plot.destroy();
            $scope.plot = null;
            $scope.dataset = null;
            $scope.Trace_Config = null;
			if($scope.IntervalProcess !== null){
				$interval.cancel($scope.IntervalProcess);
				$scope.IntervalProcess = null;
			}
            $scope.$emit("close-Trace");
        }
    };

    $scope.stopTrace = function () {
        $log.log("[ctrl-trace-handler]: stop trace");
        $scope.dataset.forEach(function (currentDataset){
           currentDataset.data = [];
        });
	    $scope.Trace_Config = null;
		if($scope.IntervalProcess !== null){
			$interval.cancel($scope.IntervalProcess);
			$scope.IntervalProcess = null;
		}
		var lan = document.getElementById("Main").className;
		document.getElementsByName(lan+"StartTrace")[0].removeAttribute("disabled");
		document.getElementsByName(lan+"StopTrace")[0].setAttribute("disabled", "true");
    };

    $scope.PlotRealtimeScope = function (TraceConfig) {
        let dataset = $scope.ReadData(TraceConfig);
            $scope.plot = $.plot("#placeholder", dataset, {
                series: {
                    lines: {
                        show: true
                    },
                    shadowSize: 1	// Drawing is faster without shadows
                },
                //colors: [ "#3058E9", "#4D8958", "#000000"],
                legend: {
                    backgroundColor: "#444444"
                },
                yaxis: {
                    color: "#ffffff",
                    AutoScale: (document.getElementById("autoScale").checked ? "true" : "false"),
					min: (document.getElementById("autoScale").checked ? null : $scope.getTraceParam('minY')),
                    max: (document.getElementById("autoScale").checked ? null : $scope.getTraceParam('maxY'))
                },
                xaxis: {
                    color: "#ffffff",
                    tickcolor: "#ffffff",
                    min: 0,
                    max: $scope.getTraceParam('totalPoints'),
                    alignTicksWithAxis: 200
                },
                grid: {
                    color: "#ffffff",
                    backgroundColor: "#000000"
                }
            });
    };

    $scope.ReadData = function(TraceConfig){
        //setup dataset
        if($scope.dataset === null){
            $scope.dataset = [];
		    if (typeof(TraceConfig.TraceConfig.Inputs) == "string")
		    {
				    let tmpDataset =
					    {
						    label: TraceConfig.TraceConfig.Inputs,
						    data: []
					    };
				    $scope.dataset.push(tmpDataset);
			
		    }
		    else 
		    {
			    TraceConfig.TraceConfig.Inputs.forEach(function(currentInput) {
				    let tmpDataset =
					    {
						    label: currentInput,
						    data: []
					    };
				    $scope.dataset.push(tmpDataset);
			    });
		    }
        }

        //check timestamp, clear data set and save the new start time
        $scope.dataset.forEach(function (currentDataSet) {
            //check data exists
            if(currentDataSet.data.length > 0){
                //check timestamp from last entry
                if(currentDataSet.data[currentDataSet.data.length - 1][0] > $scope.getTraceParam('totalPoints')){
                    //clear dataset
                    currentDataSet.data = [];
                    $scope.startTime = $scope.TPS_Values.Time;
                }
            }
        });


        //add new value
        $scope.dataset.forEach(function (currentDataset) {
            //search the value id from currentDataset TPS
            let idOfTps;
            $scope.TPS_Config.TestPointedSignals.forEach(function (currentTPS,indexTPS) {
                if(currentTPS.Name === currentDataset.label){
                    idOfTps = indexTPS;
                }
            });

            //create timestamp resolution 0.1s or 100ms
            let timestamp = ($scope.TPS_Values.Time - $scope.startTime) / 1000;
            if(timestamp < 0){                              //stop of plc
                currentDataset.data = [];                   //clear dataset
                $scope.startTime = $scope.TPS_Values.Time   //set new startTime
            }

            if ((typeof ($scope.TPS_Values.TestPointedSignals) !== "undefined") && (typeof ($scope.TPS_Values.TestPointedSignals[idOfTps]) !== "undefined") && (typeof ($scope.TPS_Values.TestPointedSignals[idOfTps].value) !== "undefined")) {
                //add new value
                currentDataset.data.push([timestamp, $scope.TPS_Values.TestPointedSignals[idOfTps].value]);
	        }
        });

        //exit
        return $scope.dataset;
    }
});

