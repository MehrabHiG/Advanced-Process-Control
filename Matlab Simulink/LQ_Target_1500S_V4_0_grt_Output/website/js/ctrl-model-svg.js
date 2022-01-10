moduleSWV.controller("ctrl-model-svg", function ($scope, $attrs, $log, $http) {
    $log.log("[ctrl-model-svg]: wurde initialisiert");

    $scope.TPS_Config = 0;
	$scope.Chart_Config = 0;

    $log.log("[ctrl-model-svg]: lade TPS config");
    $http.get("param/Model_TestPointedSignals.json",{headers:{'If-Modified-Since': '0',
    "Pragma": "no-cache",
    "Expires": -1,
    "Cache-Control": "no-cache, no-store, must-revalidate"}}).then(function (response) {
        $scope.TPS_Config = response.data;
        $log.log($scope.TPS_Config);
    });
	

    $scope.$on("open-SVG", function (event, target) {
		var divEl = document.getElementById("content");
		divEl.innerHTML = '<object id="SVG_Graphic" data-ng-controller="ctrl-model-svg" type="image/svg+xml" data=' + target + '></object>';
		// TODO: A bit risky to directly refresh the HTML content (Non-event functions in this scope will not work). 
		// Will be improved in the future (Currently necessary to avoid the empty page problem)
    });

	
	
    $scope.$on("edit-CHARTS", function (event) {
		try {
			let svg = document.getElementById("SVG_Graphic");
			let svgDoc = svg.contentDocument;
			if (svgDoc !== null && typeof ($scope.Chart_Values.StflwChartStates) !== 'undefined') {
			   //check path
			  chartpath=svg.getAttribute("data");
			  chartpath=chartpath.split("./svg/");
			  chartpath=chartpath[1];
			  chartpath=chartpath.split(".svg");
			  chartpath=chartpath[0];
			  //if(chartpath!==$scope.oldChart){
				  //remove old marking 
			  //}
			   // get the Statevalue from CPU and Map it to a StateName
			   var StateName = " ";
			   var i;
			   var actualoldStateNames=$scope.oldState;
			   var newStateNames="-1";
			   actualoldStateNames=actualoldStateNames.split("-");
			   $scope.oldState="-1";
			   $scope.oldChart="-1";
			   for (i = 0; i < $scope.Chart_Values.StflwChartStates.length; i++) {
			       var varPath = $scope.Chart_Values.StflwChartStates[i].value;
				 varPath = varPath.split("&#x27;"); // hex-ASCII ==> '
				 varPath=varPath[1];
				 varPath=varPath.split("&#x3a;"); // hex-ASCII ==> :
				 StateName=varPath[1];
				 varPath=varPath[0];
				 while (varPath.indexOf("/")!== -1){
					 varPath=varPath.replace("/","_");
				 }
				 while (varPath.indexOf("&#x20;") !== -1) {
				     varPath = varPath.replace("&#x20;", "");
				 }

				 //&& !actualoldStateNames.includes(StateName)

				 if (varPath == chartpath) {
					 if (newStateNames=="-1"){
						newStateNames=StateName;
					 }else{
						newStateNames=newStateNames.concat("-");
						newStateNames=newStateNames.concat(StateName);
					 }
			     }

			   }
			   $scope.oldState = newStateNames;
			   
			   newStateNames=newStateNames.split("-");
			   // remove no more marked States
			   for(k = 0; k < actualoldStateNames.length; k++){
//				   if(!newStateNames.includes(actualoldStateNames[k])){
						// unmark old States if exist
						m_StFlwSate = svgDoc.getElementById(actualoldStateNames[k]);
						if(m_StFlwSate!==null){
							m_StFlwSate.setAttribute("stroke","black");
							m_StFlwSate.setAttribute("stroke-width","1");
//						}
				   }
			   }
			   
			   // add new marked States
			   for(k = 0; k < newStateNames.length; k++){
//				   if(!actualoldStateNames.includes(newStateNames[k])){
						// unmark old States if exist
						m_StFlwSate = svgDoc.getElementById(newStateNames[k]);
						if(m_StFlwSate!==null){
							m_StFlwSate.setAttribute("stroke","blue");
							m_StFlwSate.setAttribute("stroke-width","3");
						}
//				   }
			   }
			   		  
			}
			else {
			  //TODO: tps not exist or undefined
			}
		}
		catch(err) {
			//TODO
		}
	});

	
	
    $scope.$on("edit-TPS", function (event) {
		try {
			let svg = document.getElementById("SVG_Graphic");
			let svgDoc = svg.contentDocument;
			if (svgDoc !== null && typeof($scope.TPS_Config.TestPointedSignals) !== 'undefined') {
			  $scope.TPS_Config.TestPointedSignals.forEach(function (currentValue, index, refarray) {
				  let tps = svgDoc.getElementById(currentValue.id);
				  if (tps !== null && (typeof($scope.TPS_Values.TestPointedSignals) !== "undefined") && (typeof($scope.TPS_Values.TestPointedSignals[index]) !== "undefined")) {
					  if (typeof($scope.TPS_Values.TestPointedSignals[index].value) == "undefined") {
						  tpsValue = " ";
					  }
					  else if (tps.getAttribute("SLType") == "boolean_T") {
						if ($scope.TPS_Values.TestPointedSignals[index].value == "1") {
							tpsValue = "true";
						}
						else { 
							tpsValue = "false";
						}
					  }
					  else {
						  tpsValue = $scope.TPS_Values.TestPointedSignals[index].value;
					  }
					  tps.textContent = tpsValue;
					  let rect = svgDoc.getElementById(currentValue.id + "_rect");
					  if (rect !== null) {
						  var bbox = tps.getBBox();
						  var padding = parseFloat(rect.getAttribute("padding"));
						  var w = bbox.width + 2*padding;
						  var w_initial = parseFloat(rect.getAttribute("initial-width"));
						  if (w < w_initial) {
							  w = w_initial;
						  }
						  rect.setAttribute("width",w);
					  }
				  }
			  });
			}
			else {
			  //TODO: tps not exist or undefined
			}
		}
		catch(err) {
			//TODO
		}
    });

});
