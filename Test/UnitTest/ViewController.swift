//
//  ViewController.swift
//  UnitTest
//
//  Created by Tomoo Hamada on 2025/03/08.
//

import BasicKit
import Cocoa

class ViewController: NSViewController {

        override func viewDidLoad() {
                super.viewDidLoad()

                // Do any additional setup after loading the view.
                if let engine = BSBasicEngine() {
                        NSLog("BasicKit: version = \(engine.version)")
                }
        }

        override var representedObject: Any? {
                didSet {
                // Update the view, if already loaded.
                }
        }


}

