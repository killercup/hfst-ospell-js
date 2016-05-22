{
  "variables": {
    "ospell_root%": "lib/hfst-ospell"
  },
  "targets": [
    {
      "target_name": "hfst-ospell",
      "sources": [
        "src/hfst-ospell.cc"
      ],
      "include_dirs" : [ 
          "<!(node -e \"require('nan')\")",
          "<(ospell_root)"
      ],
      "link_settings": {
        # "libraries": [
        #   "-llibhfstospell",
        #   "-llibhfstospell_la-ZHfstOspeller"
        # ],
        # "ld_flags": [
        #     "-L<(ospell_root)/.libs",
        #     "-Wl,-rpath,<(ospell_root)",
        # ]
      }
    }
  ]
}