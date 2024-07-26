from dash import Dash,dcc,html,callback,Input,Output,State, no_update

def get_callbacks(app):
    """"""

    @app.callback(
    Output('div-summary', 'children'),
    Input('data-store', 'data')
    )
    def update_summary_numbers(data):
        """"""