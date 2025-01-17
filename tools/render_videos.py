#!/usr/bin/python3
# coding=utf-8
# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt
#
import os
import re
import sys
import shutil
import subprocess

# ------------------------------------------------------------------------------
# The fallback console GUI
class FallbackUI:
    def __init__(self, options):
        self.title = options.example_name
    def __enter__(self): return self
    def __exit__(self, type, value, traceback): pass

    # Simple progress class for quick actions
    class SimpleProgress:
        def __init__(self, title): sys.stdout.write(title + " ... ")
        def __enter__(self): return self
        def __exit__(self, type, value, traceback):
            sys.stdout.write("Done\n")

    def simple_action(self, title):
        return FallbackUI.SimpleProgress(title)

    # Progress class for the framedump action
    class FramedumpProgress:
        def __init__(self, title): print(title)
        def __enter__(self): return self
        def __exit__(self, type, value, traceback):
            print("Done")

        def update(self, frame_no, frame_path):
            print("Rendered frame: %d" % frame_no)

    def framedump(self, title):
        return FallbackUI.FramedumpProgress(title)

    # Progress class for the video encoding action
    class VideoEncProgress:
        def __init__(self, title): print(title)
        def __enter__(self): return self
        def __exit__(self, type, value, traceback):
            print("Done")

        def update(self, message):
            print(message)

    def videoenc(self, title):
        return FallbackUI.VideoEncProgress(title)

# ------------------------------------------------------------------------------
# wxPython-based GUI
try:
    class wxPyGUI:
        import wx, threading

        # The main frame of the GUI
        class MainFrame(wx.Frame):
            def __init__(self, options):
                import wx

                wx.Frame.__init__(
                    self,
                    None,
                    wx.ID_ANY,
                    "Rendering video of '%s'" %
                    options.example_name,
                    wx.DefaultPosition,
                    wx.Size(400, 170),
                    wx.CAPTION | wx.CLIP_CHILDREN
                )
                border_sizer = wx.BoxSizer(wx.HORIZONTAL)
                border_sizer.AddSpacer(8)

                sizer = wx.BoxSizer(wx.VERTICAL)
                sizer.AddSpacer(16);

                sizer.Add(
                    wx.StaticText(
                        self,
                        wx.ID_ANY,
                        "Work directory: '%s'" %
                        options.work_dir
                    ), 0, wx.EXPAND
                )
                sizer.Add(
                    wx.StaticText(
                        self,
                        wx.ID_ANY,
                        "Bin directory: '%s'" %
                        options.bin_dir
                    ), 0, wx.EXPAND
                )
                sizer.Add(
                    wx.StaticText(
                        self,
                        wx.ID_ANY,
                        "Example: '%s'" %
                        options.example
                    ), 0, wx.EXPAND
                )
                sizer.Add(
                    wx.StaticText(
                        self,
                        wx.ID_ANY,
                        "Frame size: %dx%d" %
                        (options.width, options.height)
                    ), 0, wx.EXPAND
                )

                sizer.AddSpacer(4)

                self.gauge = wx.Gauge(self, wx.ID_ANY)
                sizer.Add(self.gauge, 0, wx.EXPAND)
                sizer.AddSpacer(4)

                self.description = wx.StaticText(self)
                sizer.Add(self.description, 0, wx.EXPAND)

                border_sizer.Add(sizer, 1, wx.EXPAND)

                border_sizer.AddSpacer(8)
                self.SetSizer(border_sizer)

                self.status_bar = wx.StatusBar(self)
                self.SetStatusBar(self.status_bar)
                self.status_bar.SetStatusText("Starting")

            def AcceptProgress(self, progress):
                progress.register(self)


        # The thread for the GUI
        class GUIThread(threading.Thread):

            def __init__(self, options):
                import wx, threading

                threading.Thread.__init__(self)
                self.initialized = threading.Event()
                self.options = options
                self.app = wx.App(False)


            def run(self):
                import wx

                self.main_frame = wxPyGUI.MainFrame(self.options)
                self.main_frame.Show()
                wx.SafeYield()
                self.initialized.set()
                self.app.MainLoop()

            def set_progress(self, progress):
                import wx
                self.initialized.wait()
                wx.CallAfter(
                    self.main_frame.AcceptProgress,
                    progress
                )

            def finish(self):
                import wx
                wx.CallAfter(self.main_frame.Destroy)

        def __init__(self, options):
            self.gui_thread = wxPyGUI.GUIThread(options)

        def __enter__(self):
            self.gui_thread.start()
            return self

        def __exit__(self, type, value, traceback):
            self.gui_thread.finish()
            self.gui_thread.join()


        # Simple progress class for quick actions
        class SimpleProgress:
            def __init__(self, ui, title):
                self.title = title
                ui.gui_thread.set_progress(self)

            def __enter__(self): return self
            def __exit__(self, type, value, traceback): pass

            def register(self, main_frame):
                import wx

                wx.CallAfter(main_frame.gauge.Pulse)
                wx.CallAfter(
                    main_frame.status_bar.SetStatusText,
                    self.title
                )
                wx.CallAfter(main_frame.gauge.Pulse)

        def simple_action(self, title):
            return wxPyGUI.SimpleProgress(self, title)


        # Progress class for the framedump action
        class FramedumpProgress(SimpleProgress):
            def __init__(self, ui, title):
                self.base = wxPyGUI.SimpleProgress
                self.base.__init__(self, ui, title)

            def register(self, main_frame):
                self.main_frame = main_frame;
                self.base.register(self, main_frame)

            def update(self, frame_no, frame_path):
                import wx

                wx.CallAfter(self.main_frame.gauge.Pulse)
                wx.CallAfter(
                    self.main_frame.description.SetLabel,
                    "Frame number: %d" % frame_no
                )
                wx.CallAfter(self.main_frame.gauge.Pulse)

        def framedump(self, title):
            return wxPyGUI.FramedumpProgress(self, title)


        # Progress class for the video encoding action
        class VideoEncProgress(SimpleProgress):
            def __init__(self, ui, title):
                self.base = wxPyGUI.SimpleProgress
                self.base.__init__(self, ui, title)

            def register(self, main_frame):
                self.main_frame = main_frame;
                self.base.register(self, main_frame)

            def update(self, message):
                import wx

                wx.CallAfter(self.main_frame.gauge.Pulse)
                wx.CallAfter(
                    self.main_frame.description.SetLabel,
                    message
                )
                wx.CallAfter(self.main_frame.gauge.Pulse)

        def videoenc(self, title):
            return wxPyGUI.VideoEncProgress(self, title)

except ImportError: pass

# ------------------------------------------------------------------------------
# Creates a user interface
def create_ui(options):
    try:
        return wxPyGUI(options)
    except NameError:
        return FallbackUI(options)


# ------------------------------------------------------------------------------
def remove_dir(work_dir):
    shutil.rmtree(work_dir)

# ------------------------------------------------------------------------------
def make_work_dir():
    from tempfile import mkdtemp
    return mkdtemp()

# ------------------------------------------------------------------------------
# returns a normalized path to the project root directory
def get_root_dir():
    import sys
    return os.path.normpath(os.path.dirname(sys.argv[0]))

# ------------------------------------------------------------------------------
# returns the path to the default build directory
def get_default_build_dir():
    try:
        try:
            if os.environ['BUILD_DIR']:
                return os.path.join(os.environ['BUILD_DIR'], 'oglplu2')
            if os.environ['BINARY_DIR']:
                return os.path.join(os.environ['BINARY_DIR'], 'oglplu2')
        except: pass

        with open(os.path.join(get_root_dir(), "BINARY_DIR"), "rt") as bdf:
            return bdf.read()
    except: return os.path.join(get_root_dir(), "_build");

# ------------------------------------------------------------------------------
def parse_args(args):
    import argparse
    import datetime

    def OutputSizeType(arg):
        if re.match("[0-9]+[kMG]?", arg):
            return arg
        msg = "'%s' is not a valid file size specification" % str(arg)
        raise argparse.ArgumentTypeError(msg)

    def FrameDimType(arg):

        try: dims = [int(dim) for dim in arg.split('x')]
        except: dims = list()

        def valid_coord(dim):
            return isinstance(dim, int) and dim > 0

        if (len(dims) == 2) and all(valid_coord(dim) for dim in dims):
            return dims
        msg = "'%s' is not a valid frame dimension specification" % str(arg)
        raise argparse.ArgumentTypeError(msg)


    argparser = argparse.ArgumentParser(
        prog=os.path.basename(args[0]),
        description="""Script for rendering videos from OGLplus examples""",
        epilog="""
            Copyright (c) 2008 - %(year)d Matúš Chochlík.
            Permission is granted to copy, distribute and/or modify this document
            under the terms of the Boost Software License, Version 1.0.
            (See a copy at http://www.boost.org/LICENSE_1_0.txt)
        """ % { "year": datetime.datetime.now().year }
    )

    argparser.add_argument(
        "--build-dir",
        help="""The name of the build directory""",
        default=get_default_build_dir(),
        action="store",
        dest="build_dir"
    )

    argparser.add_argument(
        "--scale",
        help="""
            Scaling factor for the render frame.
        """,
        type=int,
        dest="render_scale",
        action="store",
        default="1"
    )

    argparser.add_argument(
        "--size",
        help="""
            The dimensions in pixels of the video frame,
            specified as WxH where W and H are positive integers.
        """,
        type=FrameDimType,
        dest="frame_size",
        action="store",
        default="852x480"
    )

    argparser.add_argument(
        "--half-hd",
        help="""Sets the dimensions of the output to 640x360.""",
        dest="frame_size",
        action="store_const",
        const=[640,360]
    )

    argparser.add_argument(
        "--hd",
        help="""Sets the dimensions of the output to 1280x720.""",
        dest="frame_size",
        action="store_const",
        const=[1280,720]
    )

    argparser.add_argument(
        "--full-hd",
        help="""Sets the dimensions of the output to 1920x1080.""",
        dest="frame_size",
        action="store_const",
        const=[1920,1080]
        
    )

    argparser.add_argument(
        "--fps",
        help="""Number of frames per second""",
        type=int,
        default="30",
        action="store",
        dest="fps"
    )

    argparser.add_argument(
        "--samples",
        help="""Number of multisampling samples""",
        type=int,
        default="0",
        action="store",
        dest="samples"
    )

    argparser.add_argument(
        "--max-bytes",
        help="""
            Maximum output video size in bytes.
        """,
        type=OutputSizeType,
        dest="max_bytes",
        action="store",
        default=None
    )

    argparser.add_argument(
        "--gif",
        help="""Render output into GIF format""",
        default=False,
        action="store_true",
        dest="gif_output"
    )

    argparser.add_argument(
        "--twitter-gif",
        help="""Render a GIF for Twitter""",
        default=False,
        action="store_true",
        dest="twitter_gif"
    )

    argparser.add_argument(
        "--sample-label",
        help="""Example label string.""",
        dest="sample_label",
        action="store",
        default=None
    )

    argparser.add_argument(
        "--version-label",
        help="""Use version number as label string.""",
        default=False,
        action="store_true",
        dest="version_label"
    )

    argparser.add_argument(
        "example_args",
        help="""
        List of example arguments to use to render.
        The example should be specified as path to the example executable
        relative to the build directory.
        """,
        nargs="*"
    )

    return argparser.parse_args()

# ------------------------------------------------------------------------------
def fix_options(options):
    if options.twitter_gif:
        options.gif_output = True
        options.frame_size[0] = 420
        options.frame_size[1] = 240
        options.fps = 24
        options.render_scale = 2
        options.max_bytes = "14500k"

    options.width = options.frame_size[0]
    options.height= options.frame_size[1]

    return options
# ------------------------------------------------------------------------------
# checks if we have everything we need to run the example
def check_example(root_dir, example):

    if not os.path.isdir(root_dir):
        msg = "Could not find directory '%s'." % root_dir
        raise Exception(msg)

    example_path = os.path.join(
        root_dir,
        "example",
        "combined",
        example,
        "combined-%s" % example
    )

    if not os.path.isfile(example_path) or not os.access(example_path, os.X_OK):
        msg = "Could not find example '%s'." % example
        raise Exception(msg)

    return example_path


# ------------------------------------------------------------------------------
# runs imagemagick convert
def run_convert(work_dir, args):

    cmd_line = ['convert'] + args

    ret = subprocess.call(cmd_line,cwd=work_dir)
    if ret < 0:
        raise RuntimeError("Convert killed by signal %d" % -ret)
    elif ret > 0:
        raise RuntimeError("Convert failed with code %d" % ret)

# ------------------------------------------------------------------------------
# runs the example, dumps the frames, renders the video
def render_video(
    example_path,
    example_args,
    main_label_file,
    sample_label_file,
    logo_file,
    options,
    ui
):

    prefix = os.path.join(options.work_dir, 'frame')

    try:
        cmd_line = [example_path] + example_args + [
            '--framedump', '%s-'%prefix,
            '--width', str(options.width * options.render_scale),
            '--height', str(options.height * options.render_scale),
            '--fixed-fps', str(options.fps),
        ]

        if options.samples > 0:
            cmd_line = cmd_line + [
                '--samples', str(options.samples)
            ]

        proc = subprocess.Popen(
            cmd_line,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=None
        )
        proc.stdin.write(('%s-\n' % prefix).encode('utf-8'))
        proc.stdin.flush()

        with ui.framedump('Rendering frames') as progress:
            frame_no = 0
            prev_frame_path_pic = str()
            while True:
                frame_path_raw = proc.stdout.readline()
                if not frame_path_raw:
                    break
                proc.stdin.write(frame_path_raw)
                proc.stdin.flush()

                frame_path_raw = frame_path_raw.decode('utf-8').rstrip()
                frame_path_pic = frame_path_raw.replace('.rgba', '.png')

                try:
                    run_convert(options.work_dir, [
                        '-size', '%dx%d' % (
                            options.width * options.render_scale,
                            options.height * options.render_scale
                        ),
                        '-depth', '8',
                        frame_path_raw,
                        '-flip',
                        '-scale', '%dx%d' % (options.width, options.height),
                        '-alpha', 'Off',
                        '-gravity', 'SouthEast',
                        main_label_file,
                        '-composite',
                        '-gravity', 'SouthEast',
                        sample_label_file,
                        '-composite',
                        '-gravity', 'SouthEast',
                        logo_file,
                        '-composite',
                        '-quality', '100',
                        frame_path_pic
                    ])
                except RuntimeError:
                    shutil.copy2(prev_frame_path_pic, frame_path_pic)

                if os.path.isfile(frame_path_raw):
                    os.remove(frame_path_raw)
                prev_frame_path_pic = frame_path_pic

                progress.update(frame_no, frame_path_pic)
                frame_no += 1
    except OSError as os_error:
        print("Failed to execute '%(cmd)s': %(error)s" % {
            "cmd": str(' ').join(cmd_line),
            "error": os_error
        })
        sys.exit(1)


    try:
        cmd_line = ['ffmpeg',
            '-loglevel', 'error',
            '-y', '-f', 'image2',
            '-i', prefix+'-%06d.png',
            '-r', str(options.fps)
        ]

        if options.max_bytes:
            cmd_line += ['-fs', options.max_bytes]

        if options.gif_output:
            cmd_line += [
                prefix+'.gif'
            ]
        else:
            cmd_line += [
                '-vcodec', 'mpeg4',
                '-b', '8000k',
                prefix+'.avi'
            ]

        proc = subprocess.Popen(
            cmd_line,
            stdin=None,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT
        )
        with ui.videoenc('Encoding video') as progress:
            while True:
                message = proc.stdout.readline().decode('utf-8').rstrip()
                if not message: break
                progress.update(message)


    except OSError as os_error:
        print("Failed to execute '%(cmd)s': %(error)s" % {
            "cmd": str(' ').join(cmd_line),
            "error": os_error
        })
        sys.exit(2)

    if options.gif_output:
        shutil.move(prefix+'.gif', 'oglplus-'+options.example_name+'.gif')
    else:
        shutil.move(prefix+'.avi', 'oglplus-'+options.example_name+'.avi')

# ------------------------------------------------------------------------------
# renders a video for a single example
def render_example(root_dir, example, example_args, options):

    options.work_dir = make_work_dir()
    options.root_dir = root_dir
    options.bin_dir = options.build_dir
    options.example = example
    options.example_name = os.path.basename(example)
    if options.sample_label is None:
        if options.version_label:
            version_path = os.path.join(
                os.path.dirname(__file__),
                os.path.pardir,
                "VERSION"
            )
            with open(version_path, "rt") as vfd:
                options.sample_label = vfd.readline()
        else:
            options.sample_label = options.example_name

    example_path = check_example(options.bin_dir, example)

    main_label = "http://oglplus.org"

    main_label_file = os.path.join(options.work_dir, 'main_label.png')
    sample_label_file = os.path.join(options.work_dir, 'sample_label.png')
    logo_file = os.path.join(options.work_dir, 'logo.png')

    with create_ui(options) as ui:

        # render the main text label
        with ui.simple_action('Rendering main label') as progress:
            run_convert(options.work_dir, [
                '-size', '%dx24'%(len(main_label)*12 + 144), 'xc:none',
                '-background', 'none',
                '-pointsize', '24',
                '-gravity', 'center',
                '-stroke', 'black',
                '-strokewidth', '7',
                '-annotate', '0', main_label,
                '-blur', '0x4',
                '-shadow', '%dx5+1+1' % int(options.width * 0.5),
                '+repage',
                '-stroke', 'none',
                '-strokewidth', '1',
                '-fill', 'white',
                '-annotate', '0', main_label,
                main_label_file
            ])

        # render the example name label
        with ui.simple_action('Rendering example label') as progress:
            run_convert(options.work_dir, [
                '-size', '%dx70'%(len(options.sample_label)*12 + 144), 'xc:none',
                '-background', 'none',
                '-pointsize', '16',
                '-gravity', 'center',
                '-stroke', 'black',
                '-strokewidth', '3',
                '-annotate', '0', options.sample_label,
                '-blur', '0x4',
                '-shadow', '%dx4+1+1' % int(options.width * 0.6),
                '+repage',
                '-stroke', 'none',
                '-strokewidth', '1',
                '-fill', 'white',
                '-annotate', '0', options.sample_label,
                sample_label_file
            ])

        # render the logo
        with ui.simple_action('Rendering logo') as progress:
            run_convert(options.work_dir, [
                '-size', '144x144', 'xc:none',
                '-background', 'white',
                '-gravity', 'center',
                '-stroke', 'white',
                '-fill', 'white',
                '-draw', 'circle 72,72, 72,144',
                '-blur', '2x2',
                '-shadow', '%dx6' % options.width,
                '+repage',
                os.path.join(options.root_dir,'doc','logo','oglplus_circular.png'),
                '-composite',
                '-adaptive-resize', '72x72',
                '-border', '16x0',
                logo_file
            ])

        # run the example and dump the frames
        render_video(
            example_path,
            example_args,
            main_label_file,
            sample_label_file,
            logo_file,
            options,
            ui
        )

        remove_dir(options.work_dir)

# ------------------------------------------------------------------------------
def main():

    options = fix_options(parse_args(sys.argv))

    render_example(
        os.path.abspath(
            os.path.join(
                os.path.dirname(sys.argv[0]),
                os.path.pardir
            )
        ),
        os.path.splitext(options.example_args[0])[0],
        options.example_args,
        options
    )

# ------------------------------------------------------------------------------
# run the main function
if __name__ == "__main__": main()
