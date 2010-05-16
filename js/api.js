OpenLayers.WmsMap = OpenLayers.Class(OpenLayers.Map, {
  
  getTitle: function() { 
    return document.title;
  },

  doResize: function( width, height ) {
    document.body.style.width = width;
    map.div.style.width = width;
    document.body.style.height = height;
    map.div.style.height = height;
    
    this.updateSize();
  },

  getProjectionCode: function() {
    if (typeof map.displayProjection === 'undefined') {
      return map.projection.projCode;  
    }
    else {
      return map.displayProjection.projCode;  
    }
  },

  getLayerList: function() {
    var layerlist = {};
    for(var i=0; i<map.layers.length; i++) {
      layerlist[i] = {};
      layerlist[i].title = map.layers[i].name;

      if (typeof map.displayProjection === 'undefined') {
        layerlist[i].extent = map.layers[i].maxExtent.toArray();
      }
      else {
        layerlist[i].extent = map.layers[i].maxExtent.transform(
          map.getProjectionObject(),
          map.displayProjection
        ).toArray();
      }
    }
    
    return layerlist;
  },

  hasLayer: function(layername) {
    return map.layers[layername] !== "undefined" && map.layers[layername] != null;
  },

  setVisibleLayers: function(visible_layers) {
    for(var i=0; i < layers.length; i++) {
      var found = false;
      for(var j=0; j < visible_layers.length; j++) {
        if (i==j) {
            found = true;
            map.layers[i].setVisibility(true);
        }
      }
      if (!found && map.layers[i].visibility) {
        map.layers[i].setVisibility(true);
      }
    }
  },

  callDone: function() {
    if (typeof OpenlayersWMS !== 'undefined') {
      OpenlayersWMS.done();  
    } 
  }, 

  CLASS_NAME: "OpenLayers.JsWmsApi"
})
