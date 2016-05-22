{
  "targets": [
    {
      "target_name": "hfst-ospell",
      "sources": [ "src/hfst-ospell.cc" ],
      "include_dirs" : [ 
          "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}