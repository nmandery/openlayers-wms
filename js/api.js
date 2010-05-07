OpenLayers.WmsMap = OpenLayers.Class(OpenLayers.Map, {
  
  getTitle: function() { return document.title;},

  
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

  CLASS_NAME: "OpenLayers.JsWmsApi"
})