//  --------------------------------------------------------------
//  simple React form
//  Copyright(C) 2019 Kaiwen Cheng. All Rights Reserved.
//  --------------------------------------------------------------
//  Disclaimers
//  NO WARRANTY FOR SOURCE CODE
//  ALL SOURCE CODE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED.

import React, { Component } from 'react';
import { Button, Form, FormGroup, Label, Input, FormText } from 'reactstrap';
import { Formik } from 'formik';
import * as Yup from "yup";

export class ContactUS extends Component {
    static displayName = ContactUS.name;

    constructor(props) {
        super(props);
        this.state = { content: [], loading: true, submited: false };
    }

    render() {
        if (this.state.submited) {
            return (
                <div>
                    Thank you for contact US.
                </div>
            );
        } else
            return (
                <div className="container">
                    <Formik
                        initialValues={{
                            email: '', name: '', recaptcha: "",
                        }}
                        validate={values => {
                            let errors = {};
                            if (!values.email) {
                                errors.email = 'Required';
                            } else if (
                                !/^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,}$/i.test(values.email)
                            ) {
                                errors.email = 'Invalid email address';
                            }
                            return errors;
                        }}
                        onSubmit={(values, { setSubmitting }) => {
                            fetch('/api/ContactUs', {
                                method: 'POST',
                                body: JSON.stringify(values),
                                headers: {
                                    'Content-Type': 'application/json'
                                },
                            })
                            .then(response => response.text())
                            .then(data => {
                                this.setState({ content: data, loading: false, submited: true });
                            });


                        }}

                        render={({ values, errors, touched, handleChange, handleBlur, handleSubmit, isSubmitting, setFieldValue }) => (
                            <form onSubmit={handleSubmit}>
                                <div class="row">
                                    <div class="col-md-2">
                                        <Label>Email </Label>
                                    </div>
                                    <div class="col-md-6">
                                    <input
                                        type="email"
                                        name="email"
                                        onChange={handleChange}
                                        onBlur={handleBlur}
                                        value={values.email}
                                    />
                                    {errors.email && touched.email && errors.email}
                                    </div>
                                </div>

                                <div class="row">
                                    <div class="col-md-2">
                                    <Label>Name</Label>
                                    </div>
                                    <div class="col-md-6">
                                    <input
                                        type="text"
                                        name="name"
                                        onChange={handleChange}
                                        onBlur={handleBlur}
                                        value={values.name}
                                            />
                                     </div>
                                </div>

                                <div class="row">
                                <button type="submit" disabled={isSubmitting} className="btn btn-primary">submit</button>
                                </div>

                            </form>
                        )} />
                </div>
            );
    }
}