{
    "targets": [
        {
            "target_name": "my_addon",
            "sources": ["my_addon.cc"],
            "include_dirs" : [ "<!(node -e \"require('nan')\")" ]
        }
    ]
}